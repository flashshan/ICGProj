#include "Core\Math\Quaternion.h"

#include "Core\Math\VectorSSE.h"

// from Unreal
// from natrix to quaternion
Quaternion::Quaternion(const Matrix &i_matrix)
{
	/*if (i_matrix.GetScaledAxis(EAxis::X).Equal(Vector3::Zero()) || i_matrix.GetScaledAxis(EAxis::Y).Equal(Vector3::Zero()) || M.GetScaledAxis(EAxis::Z).IsNearlyZero())
	{
	*this = Quaternion::Identity();
	return;
	}*/

	// Make sure the Rotation part of the Matrix is unit length.
	// Changed to this (same as RemoveScaling) from RotDeterminant as using two different ways of checking unit length matrix caused inconsistency. 
	if ((Math::AbsF(1.0f - i_matrix.GetScaledAxis(EAxis::X).LengthSquare()) > Constants::RELATIVE_SMALL_NUMBER) || (Math::AbsF(1.0f - i_matrix.GetScaledAxis(EAxis::Y).LengthSquare()) <= Constants::RELATIVE_SMALL_NUMBER)
		|| (Math::AbsF(1.0f - i_matrix.GetScaledAxis(EAxis::Z).LengthSquare()) <= Constants::RELATIVE_SMALL_NUMBER))
	{
		*this = Quaternion::Identity();
		return;
	}

	//const MeReal *const t = (MeReal *) tm;
	float s;

	// Check diagonal (trace)
	const float tr = i_matrix.M[0][0] + i_matrix.M[1][1] + i_matrix.M[2][2];

	if (tr > 0.0f)
	{
		float InvS = Math::InvSqrt(tr + 1.0f);
		this->W = 0.5f * (1.f / InvS);
		s = 0.5f * InvS;

		this->X = (i_matrix.M[1][2] - i_matrix.M[2][1]) * s;
		this->Y = (i_matrix.M[2][0] - i_matrix.M[0][2]) * s;
		this->Z = (i_matrix.M[0][1] - i_matrix.M[1][0]) * s;
	}
	else
	{
		// diagonal is negative
		int32 i = 0;

		if (i_matrix.M[1][1] > i_matrix.M[0][0])
			i = 1;

		if (i_matrix.M[2][2] > i_matrix.M[i][i])
			i = 2;

		static const int32 nxt[3] = { 1, 2, 0 };
		const int32 j = nxt[i];
		const int32 k = nxt[j];

		s = i_matrix.M[i][i] - i_matrix.M[j][j] - i_matrix.M[k][k] + 1.0f;

		float InvS = Math::InvSqrt(s);

		float qt[4];
		qt[i] = 0.5f * (1.f / InvS);

		s = 0.5f * InvS;

		qt[3] = (i_matrix.M[j][k] - i_matrix.M[k][j]) * s;
		qt[j] = (i_matrix.M[i][j] + i_matrix.M[j][i]) * s;
		qt[k] = (i_matrix.M[i][k] + i_matrix.M[k][i]) * s;

		this->X = qt[0];
		this->Y = qt[1];
		this->Z = qt[2];
		this->W = qt[3];
	}
}

// from rotator to quaternion
Quaternion::Quaternion(const Rotator &i_rotator)
{
	// use Vector Intrinsics features, supported on Windows
	const VectorSSE angles = VectorSSE(i_rotator.Pitch, i_rotator.Yaw, i_rotator.Roll, 0.0f);
	const VectorSSE halfAngles = VectorSSE::Multiply(angles, VectorSSEConstants::DEG_TO_RAD_HALF);

	VectorSSE SinAngles, CosAngles;
	Math::VectorSinCos(&SinAngles, &CosAngles, &halfAngles);

	// Vectorized conversion, measured 20% faster than using scalar version after VectorSinCos.
	// Indices within VectorRegister (for shuffles): P=0, Y=1, R=2
	const VectorSSE SR = SinAngles.Replicate(2);
	const VectorSSE CR = CosAngles.Replicate(2);

	const VectorSSE SY_SY_CY_CY_Temp = VectorSSE(SHUFFLE(SinAngles.XYZW, CosAngles.XYZW, 1, 1, 1, 1));

	const VectorSSE SP_SP_CP_CP = VectorSSE(SHUFFLE(SinAngles.XYZW, CosAngles.XYZW, 0, 0, 0, 0));
	const VectorSSE SY_CY_SY_CY = VectorSSE(SHUFFLE(SY_SY_CY_CY_Temp.XYZW, SY_SY_CY_CY_Temp.XYZW, 0, 2, 0, 2));

	const VectorSSE CP_CP_SP_SP = VectorSSE(SHUFFLE(CosAngles.XYZW, SinAngles.XYZW, 0, 0, 0, 0));
	const VectorSSE CY_SY_CY_SY = VectorSSE(SHUFFLE(SY_SY_CY_CY_Temp.XYZW, SY_SY_CY_CY_Temp.XYZW, 2, 0, 2, 0));

	const uint32 Neg = uint32(1 << 31);
	const uint32 Pos = uint32(0);
	const VectorSSE SignBitsLeft = VectorSSE::CreateFromUint32(Pos, Neg, Pos, Pos);
	const VectorSSE SignBitsRight = VectorSSE::CreateFromUint32(Neg, Neg, Neg, Pos);
	const VectorSSE LeftTerm = VectorSSE::BitwiseXor(SignBitsLeft, VectorSSE::Multiply(CR, VectorSSE::Multiply(SP_SP_CP_CP, SY_CY_SY_CY)));
	const VectorSSE RightTerm = VectorSSE::BitwiseXor(SignBitsRight, VectorSSE::Multiply(SR, VectorSSE::Multiply(CP_CP_SP_SP, CY_SY_CY_SY)));

	const VectorSSE Result = VectorSSE::Add(LeftTerm, RightTerm);
	Result.GetFloatArray(reinterpret_cast<float *>(&X));
}

// from quaternion to rotator
Rotator Quaternion::ToRotator() const
{
	const float singularityTest = Z * X - W * Y;
	const float yawY = 2.f * (W * Z + X * Y);
	const float yawX = (1.f - 2.f * (Y * Y + Z * Z));

	// reference 
	// http://en.wikipedia.org/wiki/Conversion_between_quaternions_and_Euler_angles
	// http://www.euclideanspace.com/maths/geometry/rotations/conversions/quaternionToEuler/

	// this value was found from experience, the above websites recommend different values
	// but that isn't the case for us, so I went through different testing, and finally found the case 
	// where both of world lives happily. 
	const float SINGULARITY_THRESHOLD = 0.4999995f;

	Rotator rotatorFromQuat;

	if (singularityTest < -SINGULARITY_THRESHOLD)
	{
		rotatorFromQuat.Pitch = -90.f;

		rotatorFromQuat.Yaw = atan2f(yawY, yawX) * Constants::RAD_TO_DEG;
		rotatorFromQuat.Roll = Rotator::NormalizeAxis(-rotatorFromQuat.Yaw - (2.f * atan2f(X, W) * Constants::RAD_TO_DEG));
	}
	else if (singularityTest > SINGULARITY_THRESHOLD)
	{
		rotatorFromQuat.Pitch = 90.f;
		rotatorFromQuat.Yaw = atan2f(yawY, yawX) * Constants::RAD_TO_DEG;
		rotatorFromQuat.Roll = Rotator::NormalizeAxis(rotatorFromQuat.Yaw - (2.f * atan2f(X, W) * Constants::RAD_TO_DEG));
	}
	else
	{
		rotatorFromQuat.Pitch = Math::FastAsin(2.f * (singularityTest)) * Constants::RAD_TO_DEG;
		rotatorFromQuat.Yaw = atan2f(yawY, yawX) * Constants::RAD_TO_DEG;
		rotatorFromQuat.Roll = atan2f(-2.f*(W*X + Y*Z), (1.f - 2.f*(X * X + Y * Y))) * Constants::RAD_TO_DEG;
	}

	return rotatorFromQuat;
}


Quaternion Quaternion::SlerpUnNormalized(const Quaternion &i_quat1, const Quaternion &i_quat2, float i_slerp)
{
	// Get cosine of angle between quats.
	const float rawCosom = i_quat1.X * i_quat2.X + i_quat1.Y * i_quat2.Y + i_quat1.Z * i_quat2.Z + i_quat1.W * i_quat2.W;

	// Unaligned quats - compensate, results in taking shorter route.
	const float cosom = (rawCosom > 0.f) ? rawCosom : -rawCosom;

	float scale0, scale1;

	if (cosom < 0.9999f)
	{
		const float omega = acosf(cosom);
		const float invSin = 1.f / sinf(omega);
		scale0 = sinf((1.f - i_slerp) * omega) * invSin;
		scale1 = sinf(i_slerp * omega) * invSin;
	}
	else
	{
		// Use linear interpolation.
		scale0 = 1.0f - i_slerp;
		scale1 = i_slerp;
	}

	// In keeping with our flipped Cosom:
	scale1 = rawCosom > 0 ? scale1 : -scale1;

	return Quaternion(scale0 * i_quat1.X + scale1 * i_quat2.X, scale0 * i_quat1.Y + scale1 * i_quat2.Y, scale0 * i_quat1.Z + scale1 * i_quat2.Z, scale0 * i_quat1.W + scale1 * i_quat2.W);
}

Quaternion Quaternion::SlerpFullPathUnNormalized(const Quaternion &i_quat1, const Quaternion &i_quat2, float i_alpha)
{
	const float cosAngle = Math::Clamp(i_quat1 | i_quat2, -1.f, 1.f);
	const float angle = acosf(cosAngle);

	//UE_LOG(LogUnrealMath, Log,  TEXT("CosAngle: %f Angle: %f"), CosAngle, Angle );
	if (Math::AbsF(angle) < Constants::RELATIVE_SMALL_NUMBER)
	{
		return i_quat1;
	}

	const float sinAngle = sinf(angle);
	const float invSinAngle = 1.f / sinAngle;

	const float scale0 = sinf((1.0f - i_alpha) * angle) * invSinAngle;
	const float scale1 = sinf(i_alpha * angle) * invSinAngle;

	return i_quat1 * scale0 + i_quat2 * scale1;
}


