#pragma once

#include "Core\Basic\BasicTypes.h"
#include "Core\Basic\Assert.h"

#include "Matrix.h"
#include "Rotator.h"

ALIGN(16) struct Quaternion {
public:
	FORCEINLINE Quaternion() {}
	FORCEINLINE Quaternion(float i_x, float i_y, float i_z, float i_w);
	explicit FORCEINLINE Quaternion(VectorSSE &i_vectorSSE);
	explicit Quaternion(const Matrix &i_matrix);
	explicit Quaternion(const Rotator &i_rotator);
	FORCEINLINE Quaternion(const Vector3 &i_axis, float i_angleRad);

	FORCEINLINE Quaternion(const Quaternion &i_other);
	FORCEINLINE Quaternion& operator=(const Quaternion &i_other);

	static FORCEINLINE Quaternion Identity();
	static FORCEINLINE Quaternion FromEular(const Vector3 &i_euler);							// long function which may assert

	// check identical
	FORCEINLINE bool operator==(const Quaternion &i_other) const;
	FORCEINLINE bool operator!=(const Quaternion &i_other) const;
	FORCEINLINE bool Equal(const Quaternion& i_other, float i_tolerance = Constants::RELATIVE_SMALL_NUMBER) const;
	FORCEINLINE bool IsIdentity(float i_tolerance = Constants::SMALL_NUMBER) const;
	FORCEINLINE bool ContainsNan() const;

	FORCEINLINE Quaternion operator +(const Quaternion &i_other) const;
	FORCEINLINE Quaternion operator -(const Quaternion &i_other) const;
	FORCEINLINE Quaternion operator *(const Quaternion &i_other) const;
	FORCEINLINE Quaternion operator *(float i_float) const;
	FORCEINLINE Quaternion operator /(float i_float) const;
	inline float operator|(const Quaternion &i_other) const;

	FORCEINLINE Quaternion& operator +=(const Quaternion &i_other);
	FORCEINLINE Quaternion& operator -=(const Quaternion &i_other);
	FORCEINLINE Quaternion& operator *=(const Quaternion &i_other);
	FORCEINLINE Quaternion& operator *=(float i_float);
	FORCEINLINE Quaternion& operator /=(float i_float);

	FORCEINLINE Vector3 operator *(const Vector3 &i_vector) const;
	inline Matrix operator *(const Matrix &i_matrix) const;

	FORCEINLINE Quaternion& Normalize(float i_tolerance = Constants::SMALL_NUMBER);
	FORCEINLINE Quaternion GetNormalized(float i_tolerance = Constants::SMALL_NUMBER) const;
	FORCEINLINE bool IsNormalized() const;

	FORCEINLINE Quaternion& Inverse();
	FORCEINLINE Quaternion GetInverse() const;

	FORCEINLINE float Length() const;
	FORCEINLINE float LengthSquare() const;

	Rotator ToRotator() const;
	FORCEINLINE Vector3 ToEuler() const;
	FORCEINLINE void ToAxisAndAngle(Vector3 &o_axis, float &o_angle);

	FORCEINLINE Vector3 RotateVector(const Vector3 &i_vector) const;
	FORCEINLINE Vector3 UnrotateVector(const Vector3 &i_vector) const;

	FORCEINLINE void EnforceShortestArcWith(const Quaternion& i_other);

	FORCEINLINE Vector3 GetAxisX() const;
	FORCEINLINE Vector3 GetAxisY() const;
	FORCEINLINE Vector3 GetAxisZ() const;

	FORCEINLINE Vector3 GetRotationAxis() const;

	FORCEINLINE Vector3 GetForwardVector() const;
	FORCEINLINE Vector3 GetRightVector() const;
	FORCEINLINE Vector3 GetUpVector() const;

	FORCEINLINE float AngularDistance(const Quaternion &i_other);

	// Slerp, result is normalized
	static FORCEINLINE Quaternion Slerp(const Quaternion &i_quat1, const Quaternion &i_quat2, float i_slerp);
	static FORCEINLINE Quaternion SlerpFullPath(const Quaternion &i_quat1, const Quaternion &i_quat2, float i_alpha);
	static FORCEINLINE Quaternion Squad(const Quaternion &i_quat1, const Quaternion& i_tang1, const Quaternion& i_quat2, const Quaternion& i_tang2, float i_alpha);
	static FORCEINLINE Quaternion SquadFullPath(const Quaternion &i_quat1, const Quaternion &i_tang1, const Quaternion &i_quat2, const Quaternion &i_tang2, float i_alpha);

	// lerp functions, result is not normalized
	static FORCEINLINE Quaternion FastLerpUnNormalized(const Quaternion &i_quat1, const Quaternion &i_quat2, float i_alpha);
	static FORCEINLINE Quaternion FastBilerpUnNormalized(const Quaternion &i_quat00, const Quaternion &i_quat10, const Quaternion &i_quat01, const Quaternion &i_quat11, float i_fracX, float i_fracY);
	static Quaternion SlerpUnNormalized(const Quaternion &i_quat1, const Quaternion &i_quat2, float i_slerp);
	static Quaternion SlerpFullPathUnNormalized(const Quaternion &i_quat1, const Quaternion &i_quat2, float i_alpha);

public:
	float X, Y, Z, W;
};







// implement forceinline

FORCEINLINE Quaternion::Quaternion(float i_x, float i_y, float i_z, float i_w)
	: X(i_x), Y(i_y), Z(i_z), W(i_w)
{
}

FORCEINLINE Quaternion::Quaternion(VectorSSE &i_vectorSSE)
{
	i_vectorSSE.GetFloatArray(&X);
}

FORCEINLINE Quaternion::Quaternion(const Vector3 &i_axis, float i_angleRad)
{
	const float halfAngle = 0.5f * i_angleRad;
	float s, c;
	Math::SinCos(&s, &c, halfAngle);

	X = s * i_axis.X;
	Y = s * i_axis.Y;
	Z = s * i_axis.Z;
	W = c;
}

FORCEINLINE Quaternion::Quaternion(const Quaternion &i_other)
	: X(i_other.X), Y(i_other.Y), Z(i_other.Z), W(i_other.W)
{
}

FORCEINLINE Quaternion& Quaternion::operator=(const Quaternion &i_other)
{
	X = i_other.X;
	Y = i_other.Y;
	Z = i_other.Z;
	W = i_other.W;
	return *this;
}

FORCEINLINE Quaternion Quaternion::Identity()
{
	return Quaternion(0.f, 0.f, 0.f, 1.f);
}

FORCEINLINE Quaternion Quaternion::FromEular(const Vector3 &i_euler)
{
	return Quaternion(Rotator::FromEuler(i_euler));
}


FORCEINLINE bool Quaternion::operator==(const Quaternion &i_other) const
{
	// use vector intrubsucs feature
	return VectorSSE::MaskBits(VectorSSE::CompareEQ(VectorSSE(*this), VectorSSE(i_other))) == 0x0F;
}

FORCEINLINE bool Quaternion::operator!=(const Quaternion &i_other) const
{
	return VectorSSE::MaskBits(VectorSSE::CompareNE(VectorSSE(*this), VectorSSE(i_other))) != 0x00;
}

FORCEINLINE bool Quaternion::Equal(const Quaternion& i_other, float i_tolerance) const
{
	// use vector intrinsics feature
	const VectorSSE toleranceV = VectorSSE(i_tolerance, i_tolerance, i_tolerance, i_tolerance);
	const VectorSSE A = VectorSSE(X,Y,Z,W);
	const VectorSSE B = VectorSSE(i_other.X, i_other.Y, i_other.Z, i_other.W);

	const VectorSSE RotationSub = VectorSSE::Subtract(A, B).GetAbs();
	const VectorSSE RotationAdd = VectorSSE::Add(A, B).GetAbs();
	return !VectorSSE::CompareAnyGT(RotationSub, toleranceV) || !VectorSSE::CompareAnyGT(RotationAdd, toleranceV);
}

FORCEINLINE bool Quaternion::IsIdentity(float i_tolerance) const
{
	return Equal(Quaternion::Identity(), i_tolerance);
}

FORCEINLINE bool Quaternion::ContainsNan() const
{
	return (Float::IsNAN(X) || Float::IsNAN(Y) || Float::IsNAN(Z) || Float::IsNAN(W));
}


FORCEINLINE Quaternion Quaternion::operator +(const Quaternion &i_other) const
{
	return Quaternion(X + i_other.X, Y + i_other.Y, Z + i_other.Z, W + i_other.W);
}

FORCEINLINE Quaternion Quaternion::operator -(const Quaternion &i_other) const
{
	return Quaternion(X - i_other.X, Y - i_other.Y, Z - i_other.Z, W - i_other.W);
}

FORCEINLINE Quaternion Quaternion::operator *(const Quaternion &i_other) const
{
	VectorSSE quatV1 = VectorSSE(*this);
	VectorSSE quatV2 = VectorSSE(i_other);

	VectorSSE result = VectorSSE::Multiply(quatV1.Replicate(3), quatV2);
	result = VectorSSE::MultiplyAdd(VectorSSE::Multiply(quatV1.Replicate(0), VectorSSE(SHUFFLE(quatV2.XYZW, quatV2.XYZW, 3, 2, 1, 0))), VectorSSEConstants::QMULTI_SIGN_MASK0, result);
	result = VectorSSE::MultiplyAdd(VectorSSE::Multiply(quatV1.Replicate(1), VectorSSE(SHUFFLE(quatV2.XYZW, quatV2.XYZW, 2, 3, 0, 1))), VectorSSEConstants::QMULTI_SIGN_MASK1, result);
	result = VectorSSE::MultiplyAdd(VectorSSE::Multiply(quatV1.Replicate(2), VectorSSE(SHUFFLE(quatV2.XYZW, quatV2.XYZW, 1, 0, 3, 2))), VectorSSEConstants::QMULTI_SIGN_MASK2, result);

	return Quaternion(result);
}

FORCEINLINE Quaternion Quaternion::operator *(float i_float) const
{
	return Quaternion(X * i_float, Y * i_float, Z * i_float, W * i_float);
}

FORCEINLINE Quaternion Quaternion::operator /(float i_float) const
{
	const float reciprocal = 1.0f / i_float;
	return Quaternion(X * reciprocal, Y * reciprocal, Z * reciprocal, W * reciprocal);
}

inline float Quaternion::operator|(const Quaternion &i_other) const
{
	return X * i_other.X + Y * i_other.Y + Z * i_other.Z + W * i_other.W;
}

FORCEINLINE Quaternion& Quaternion::operator +=(const Quaternion &i_other)
{
	X += i_other.X;
	Y += i_other.Y;
	Z += i_other.Z;
	W += i_other.W;
	return *this;
}

FORCEINLINE Quaternion& Quaternion::operator -=(const Quaternion &i_other)
{
	X -= i_other.X;
	Y -= i_other.Y;
	Z -= i_other.Z;
	W -= i_other.W;
	return *this;
}

FORCEINLINE Quaternion& Quaternion::operator *=(const Quaternion &i_other)
{
	VectorSSE quatV1 = VectorSSE(*this);
	VectorSSE quatV2 = VectorSSE(i_other);

	VectorSSE result = VectorSSE::Multiply(quatV1.Replicate(3), quatV2);
	result = VectorSSE::MultiplyAdd(VectorSSE::Multiply(quatV1.Replicate(0), VectorSSE(SHUFFLE(quatV2.XYZW, quatV2.XYZW, 3, 2, 1, 0))), VectorSSEConstants::QMULTI_SIGN_MASK0, result);
	result = VectorSSE::MultiplyAdd(VectorSSE::Multiply(quatV1.Replicate(1), VectorSSE(SHUFFLE(quatV2.XYZW, quatV2.XYZW, 2, 3, 0, 1))), VectorSSEConstants::QMULTI_SIGN_MASK1, result);
	result = VectorSSE::MultiplyAdd(VectorSSE::Multiply(quatV1.Replicate(2), VectorSSE(SHUFFLE(quatV2.XYZW, quatV2.XYZW, 1, 0, 3, 2))), VectorSSEConstants::QMULTI_SIGN_MASK2, result);

	result.GetFloatArray(&X);
	return *this;
}

FORCEINLINE Quaternion& Quaternion::operator *=(float i_float)
{
	X *= i_float;
	Y *= i_float;
	Z *= i_float;
	W *= i_float;
	return *this;
}

FORCEINLINE Quaternion& Quaternion::operator /=(float i_float)
{
	const float reciprocal = 1.0f / i_float;
	X *= reciprocal;
	Y *= reciprocal;
	Z *= reciprocal;
	W *= reciprocal;
	return *this;
}

FORCEINLINE Vector3 Quaternion::operator *(const Vector3 &i_vector) const
{
	return RotateVector(i_vector);
}

inline Matrix Quaternion::operator *(const Matrix &i_matrix) const
{
	Matrix result;

	Quaternion Inv = GetInverse();
	for (int32 i = 0; i<4; ++i)
	{
		Quaternion tempQuat(i_matrix.M[i][0], i_matrix.M[i][1], i_matrix.M[i][2], i_matrix.M[i][3]);
		tempQuat = *this * tempQuat * Inv;
		result.M[i][0] = tempQuat.X;
		result.M[i][1] = tempQuat.Y;
		result.M[i][2] = tempQuat.Z;
		result.M[i][3] = tempQuat.W;
	}

	return result;
}

FORCEINLINE Quaternion& Quaternion::Normalize(float i_tolerance)
{
	// use vector intrinsics
	const VectorSSE vectorQuat(*this);
	const VectorSSE squareSum = VectorSSE::Dot4(vectorQuat, vectorQuat);
	const VectorSSE nonZeroMask = VectorSSE::CompareGE(squareSum, VectorSSE(i_tolerance));
	const VectorSSE invLength = squareSum.ReciprocalSqrtAccurate();
	const VectorSSE normalizedVector = VectorSSE::Multiply(invLength, vectorQuat);
	VectorSSE result = VectorSSE::VectorSelect(nonZeroMask, normalizedVector, VectorSSEConstants::Float0001);

	result.GetFloatArray(&X);
	return *this;
}

FORCEINLINE Quaternion Quaternion::GetNormalized(float i_tolerance) const
{
	Quaternion result(*this);
	return result.Normalize();
}

FORCEINLINE bool Quaternion::IsNormalized() const
{
	return (Math::AbsF(1.0f - LengthSquare()) < VectorSSEConstants::allowedErrorForQuaternion);
}

FORCEINLINE Quaternion& Quaternion::Inverse()
{
	ASSERT(IsNormalized());
	X = -X;
	Y = -Y;
	Z = -Z;
	return *this;
}

FORCEINLINE Quaternion Quaternion::GetInverse() const
{
	ASSERT(IsNormalized());
	return Quaternion(-X, -Y, -Z, W);
}

FORCEINLINE float Quaternion::Length() const
{
	return sqrtf(X * X + Y * Y + Z * Z + W * W);
}

FORCEINLINE float Quaternion::LengthSquare() const
{
	return X * X + Y * Y + Z * Z + W * W;
}


FORCEINLINE Vector3 Quaternion::ToEuler() const
{
	ToRotator().ToEuler();
}

FORCEINLINE void Quaternion::ToAxisAndAngle(Vector3 &o_axis, float &o_angle)
{
	o_angle = 2.f * acosf(W);
	o_axis = GetRotationAxis();
}

FORCEINLINE Vector3 Quaternion::RotateVector(const Vector3 &i_vector) const
{
	// http://people.csail.mit.edu/bkph/articles/Quaternions.pdf
	// V' = V + 2w(Q x V) + (2Q x (Q x V))
	// refactor:
	// V' = V + w(2(Q x V)) + (Q x (2(Q x V)))
	// T = 2(Q x V);
	// V' = V + w*(T) + (Q x T)

	const Vector3 vectorQ(X, Y, Z);
	const Vector3 vectorT = Vector3::Cross(vectorQ, i_vector) * 2.f;
	const Vector3 Result = i_vector + (vectorT * W) + Vector3::Cross(vectorQ, vectorT);
	return Result;
}

FORCEINLINE Vector3 Quaternion::UnrotateVector(const Vector3 &i_vector) const
{
	//return Inverse().RotateVector(V);
	const Vector3 vectorQ(-X, -Y, -Z);
	const Vector3 vectorT = Vector3::Cross(vectorQ, i_vector) * 2.f;
	const Vector3 Result = i_vector + (vectorT * W) + Vector3::Cross(vectorQ, vectorT);
	return Result;
}

FORCEINLINE void Quaternion::EnforceShortestArcWith(const Quaternion& i_other)
{
	const float dotResult = (*this|i_other);
	const float Bias = (dotResult > 0.f) ? 1.f : -1.f;

	X *= Bias;
	Y *= Bias;
	Z *= Bias;
	W *= Bias;
}


FORCEINLINE Vector3 Quaternion::GetAxisX() const
{
	return RotateVector(Vector3(1.f, 0.f, 0.f));
}

FORCEINLINE Vector3 Quaternion::GetAxisY() const
{
	return RotateVector(Vector3(0.f, 1.f, 0.f));
}

FORCEINLINE Vector3 Quaternion::GetAxisZ() const
{
	return RotateVector(Vector3(0.f, 0.f, 1.f));
}

FORCEINLINE Vector3 Quaternion::GetRotationAxis() const
{
	// Ensure we never try to sqrt a neg number
	const float S = sqrtf(Math::Max<float>(1.f - (W * W), 0.f));

	ASSERT(S > Constants::RELATIVE_SMALL_NUMBER);
	return Vector3(X / S, Y / S, Z / S);
}


FORCEINLINE Vector3 Quaternion::GetForwardVector() const
{
	return GetAxisX();
}

FORCEINLINE Vector3 Quaternion::GetRightVector() const
{
	return GetAxisY();
}

FORCEINLINE Vector3 Quaternion::GetUpVector() const
{
	return GetAxisZ();
}


FORCEINLINE float Quaternion::AngularDistance(const Quaternion &i_other)
{
	float InnerProd = *this|i_other;
	return acosf((2 * InnerProd * InnerProd) - 1.f);
}


FORCEINLINE Quaternion Quaternion::Slerp(const Quaternion &i_quat1, const Quaternion &i_quat2, float i_slerp)
{
	return SlerpUnNormalized(i_quat1, i_quat2, i_slerp).Normalize();
}

FORCEINLINE Quaternion Quaternion::SlerpFullPath(const Quaternion &i_quat1, const Quaternion &i_quat2, float i_alpha)
{
	return SlerpFullPathUnNormalized(i_quat1, i_quat2, i_alpha).Normalize();
}

FORCEINLINE Quaternion Quaternion::Squad(const Quaternion &i_quat1, const Quaternion& i_tang1, const Quaternion& i_quat2, const Quaternion& i_tang2, float i_alpha)
{
	// Always slerp along the short path from quat1 to quat2 to prevent axis flipping.
	// This approach is taken by OGRE engine, amongst others.
	const Quaternion quat1 = Quaternion::SlerpUnNormalized(i_quat1, i_quat2, i_alpha);
	const Quaternion quat2 = Quaternion::SlerpFullPathUnNormalized(i_tang1, i_tang2, i_alpha);
	return Quaternion::SlerpFullPath(quat1, quat2, 2.f * i_alpha * (1.f - i_alpha));
}

FORCEINLINE Quaternion Quaternion::SquadFullPath(const Quaternion &i_quat1, const Quaternion &i_tang1, const Quaternion &i_quat2, const Quaternion &i_tang2, float i_alpha)
{
	const Quaternion quat1 = Quaternion::SlerpFullPathUnNormalized(i_quat1, i_quat2, i_alpha);
	const Quaternion quat2 = Quaternion::SlerpFullPathUnNormalized(i_tang1, i_tang2, i_alpha);
	return Quaternion::SlerpFullPath(quat1, quat2, 2.f * i_alpha * (1.f - i_alpha));
}



FORCEINLINE Quaternion Quaternion::FastLerpUnNormalized(const Quaternion &i_quat1, const Quaternion &i_quat2, float i_alpha)
{
	const float bias = ((i_quat1 | i_quat2) > 0.f) ? 1.f : -1.f;
	return (i_quat2 * i_alpha) + (i_quat1 * (bias * (1.f - i_alpha)));
}

FORCEINLINE Quaternion Quaternion::FastBilerpUnNormalized(const Quaternion &i_quat00, const Quaternion &i_quat10, const Quaternion &i_quat01, const Quaternion &i_quat11, float i_fracX, float i_fracY)
{
	return FastLerpUnNormalized( FastLerpUnNormalized(i_quat00, i_quat10, i_fracX), FastLerpUnNormalized(i_quat01, i_quat11, i_fracX), i_fracY);
}





// for math
FORCEINLINE Quaternion Math::Lerp(const Quaternion &i_quat1, const Quaternion &i_quat2, float i_alpha)
{
	return Quaternion::Slerp(i_quat1, i_quat2, i_alpha);
}

FORCEINLINE Quaternion Math::BiLerp(const Quaternion& i_quat00, const Quaternion& i_quat10, const Quaternion& i_quat01, const Quaternion& i_quat11, float i_fracX, float i_fracY)
{
	return Math::Lerp(Quaternion::SlerpUnNormalized(i_quat00, i_quat10, i_fracX), Quaternion::SlerpUnNormalized(i_quat01, i_quat11, i_fracX), i_fracY);
}

FORCEINLINE Quaternion Math::CubicInterp(const Quaternion& i_quatP0, const Quaternion& i_quatT0, const Quaternion& i_quatP1, const Quaternion& i_quatT1, float i_alpha)
{
	return Quaternion::Squad(i_quatP0, i_quatT0, i_quatP1, i_quatT1, i_alpha);
}


// for VectorSSE

FORCEINLINE VectorSSE::VectorSSE(const Quaternion &i_quat)
	: XYZW(_mm_setr_ps(i_quat.X, i_quat.Y, i_quat.Z, i_quat.W))
{
}


// for Rotator
FORCEINLINE Quaternion Rotator::ToQuaternion() const
{
	return Quaternion(*this);
}

// for matrix
FORCEINLINE Quaternion Matrix::ToQuaternion() const
{
	return Quaternion(*this);
}
