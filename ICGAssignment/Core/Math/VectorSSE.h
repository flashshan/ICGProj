#pragma once


#include "Vector4.h"

struct Matrix;

// an encapsulation of __m128
ALIGN(16) struct VectorSSE {
public:
	VectorSSE() {}
	explicit FORCEINLINE VectorSSE(const float *i_array);
	FORCEINLINE VectorSSE(float i_x, float i_y, float i_z, float i_w = 1.0f);                  // assign by 4 values
	explicit FORCEINLINE VectorSSE(float i_float);
	explicit FORCEINLINE VectorSSE(__m128 i_xyzw);
	explicit FORCEINLINE VectorSSE(const Vector2 & i_vector, float i_z = 0.0f, float i_w = 1.0f);
	explicit FORCEINLINE VectorSSE(const Vector3 & i_vector, float i_w = 1.0f);
	explicit FORCEINLINE VectorSSE(const Quaternion &i_quat);
	explicit FORCEINLINE VectorSSE(const Vector4 &i_vector);

	FORCEINLINE VectorSSE(const VectorSSE &i_other);
	FORCEINLINE VectorSSE& operator=(const VectorSSE& i_other);

	static FORCEINLINE VectorSSE CreateFromUint32(uint32 i_x, uint32 i_y, uint32 i_z, uint32 i_w);        // assign by 32 * 4 bits

	FORCEINLINE void GetFloatArray(float *o_array) const;
	FORCEINLINE void GetFloatArrayUnAligned(float *o_array) const;

	FORCEINLINE VectorSSE Replicate(uint32 i_index) const;
	
	FORCEINLINE VectorSSE LengthSquare3() const;
	FORCEINLINE VectorSSE LengthSquare4() const;
	FORCEINLINE VectorSSE Length3() const;
	FORCEINLINE VectorSSE Length4() const;
	FORCEINLINE VectorSSE& Normalize3(const float i_tolerance = Constants::SMALL_NUMBER);
	FORCEINLINE VectorSSE& Normalize4(const float i_tolerance = Constants::SMALL_NUMBER);

	FORCEINLINE VectorSSE GetAbs() const;
	FORCEINLINE VectorSSE Negate() const;

	static FORCEINLINE VectorSSE Add(const VectorSSE &i_vector1, const VectorSSE &i_vector2);
	static FORCEINLINE VectorSSE Add(const VectorSSE &i_vector, float i_float);
	static FORCEINLINE VectorSSE Subtract(const VectorSSE &i_vector1, const VectorSSE &i_vector2);
	static FORCEINLINE VectorSSE Subtract(const VectorSSE &i_vector, float i_float);
	static FORCEINLINE VectorSSE Multiply(const VectorSSE &i_vector1, const VectorSSE &i_vector2);
	static FORCEINLINE VectorSSE Multiply(const VectorSSE &i_vector, float i_float);
	static FORCEINLINE VectorSSE Divide(const VectorSSE &i_vector1, const VectorSSE &i_vector2);
	static FORCEINLINE VectorSSE Divide(const VectorSSE &i_vector, float i_float);
	
	static FORCEINLINE VectorSSE MultiplyAdd(const VectorSSE &i_vector1, const VectorSSE &i_vector2, const VectorSSE &i_vector3);

	static FORCEINLINE VectorSSE Dot3(const VectorSSE &i_vector1, const VectorSSE &i_vector2);
	static FORCEINLINE VectorSSE Dot4(const VectorSSE &i_vector1, const VectorSSE &i_vector2);
	static FORCEINLINE VectorSSE Cross3(const VectorSSE &i_vector1, const VectorSSE &i_vector2);

	static FORCEINLINE VectorSSE VectorSelect(const VectorSSE &i_mask, const VectorSSE &i_vector1, const VectorSSE &i_vector2);
	static FORCEINLINE VectorSSE BitwiseOr(const VectorSSE &i_vector1, const VectorSSE &i_vector2);
	static FORCEINLINE VectorSSE BitwiseAnd(const VectorSSE &i_vector1, const VectorSSE &i_vector2);
	static FORCEINLINE VectorSSE BitwiseXor(const VectorSSE &i_vector1, const VectorSSE &i_vector2);
	
	static FORCEINLINE VectorSSE CompareEQ(const VectorSSE &i_vector1, const VectorSSE &i_vector2);
	static FORCEINLINE VectorSSE CompareNE(const VectorSSE &i_vector1, const VectorSSE &i_vector2);
	static FORCEINLINE VectorSSE CompareGT(const VectorSSE &i_vector1, const VectorSSE &i_vector2);
	static FORCEINLINE VectorSSE CompareGE(const VectorSSE &i_vector1, const VectorSSE &i_vector2);
	static FORCEINLINE VectorSSE CompareLT(const VectorSSE &i_vector1, const VectorSSE &i_vector2);
	static FORCEINLINE VectorSSE CompareLE(const VectorSSE &i_vector1, const VectorSSE &i_vector2);
	static FORCEINLINE int32 CompareAnyGT(const VectorSSE &i_vector1, const VectorSSE &i_vector2);
	
	static FORCEINLINE int32 MaskBits(const VectorSSE &i_vector);

	static FORCEINLINE VectorSSE Min(const VectorSSE &i_vector1, const VectorSSE &i_vector2);
	static FORCEINLINE VectorSSE Max(const VectorSSE &i_vector1, const VectorSSE &i_vector2);
	static FORCEINLINE VectorSSE Mod(const VectorSSE &i_vector1, const VectorSSE &i_vector2);

	FORCEINLINE VectorSSE Reciprocal() const;
	FORCEINLINE VectorSSE ReciprocalLen(const VectorSSE& i_vector) const;
	FORCEINLINE VectorSSE ReciprocalSqrtAccurate() const;
	FORCEINLINE VectorSSE VectorReciprocalAccurate() const;

	FORCEINLINE VectorSSE Sqrt() const;
	FORCEINLINE VectorSSE ReciprocalSqrt() const;

	FORCEINLINE VectorSSE Truncate() const;
	FORCEINLINE VectorSSE Fractional() const;
	FORCEINLINE VectorSSE Ceil() const;
	FORCEINLINE VectorSSE Floor() const;

	FORCEINLINE float operator[](uint32 i_index) const;

	FORCEINLINE VectorSSE Mul(const Matrix &i_matrix) const;

	FORCEINLINE VectorSSE VectorSin();
	FORCEINLINE VectorSSE VectorCos();

public:
	__m128 XYZW;
};

// from Unreal

namespace VectorSSEConstants
{
	static const VectorSSE FloatOne = VectorSSE(1.0f);
	static const VectorSSE FloatZero = VectorSSE(0.0f);
	static const VectorSSE FloatMinusOne = VectorSSE(-1.0f, -1.0f, -1.0f, -1.0f);
	static const VectorSSE Float0001 = VectorSSE(0.0f, 0.0f, 0.0f, 1.0f);
	static const VectorSSE SmallLengthThreshold = VectorSSE(1.e-8f);
	static const VectorSSE FloatOneHundredth = VectorSSE(0.01f);
	static const VectorSSE Float111_Minus1 = VectorSSE(1.f, 1.f, 1.f, -1.f);
	static const VectorSSE FloatMinus1_111 = VectorSSE(-1.f, 1.f, 1.f, 1.f);
	static const VectorSSE FloatOneHalf = VectorSSE(0.5f);
	static const VectorSSE FloatMinusOneHalf = VectorSSE(-0.5f);
	static const VectorSSE KindaSmallNumber = VectorSSE(Constants::RELATIVE_SMALL_NUMBER);
	static const VectorSSE SmallNumber = VectorSSE(Constants::SMALL_NUMBER);
	
	static const float allowedErrorForQuaternion = 0.01f;
	static const VectorSSE ThreshQuatNormalized = VectorSSE(allowedErrorForQuaternion);
	static const VectorSSE BigNumber = VectorSSE(Constants::BIG_NUMBER);

	/** This is to speed up Quaternion Inverse. Static variable to keep sign of inverse **/
	static const VectorSSE QINV_SIGN_MASK = VectorSSE(-1.f, -1.f, -1.f, 1.f);

	static const VectorSSE QMULTI_SIGN_MASK0 = VectorSSE(1.f, -1.f, 1.f, -1.f);
	static const VectorSSE QMULTI_SIGN_MASK1 = VectorSSE(1.f, 1.f, -1.f, -1.f);
	static const VectorSSE QMULTI_SIGN_MASK2 = VectorSSE(-1.f, 1.f, 1.f, -1.f);

	static const VectorSSE DEG_TO_RAD = VectorSSE(Constants::Pi / (180.f));
	static const VectorSSE DEG_TO_RAD_HALF = VectorSSE((Constants::Pi / 180.f)*0.5f);
	static const VectorSSE RAD_TO_DEG = VectorSSE((180.f) / Constants::Pi);

	/** Bitmask to AND out the XYZ components in a vector */
	static const VectorSSE XYZMask = VectorSSE::CreateFromUint32(0xffffffff, 0xffffffff, 0xffffffff, 0x00000000);

	/** Bitmask to AND out the sign bit of each components in a vector */
	static const VectorSSE SignBit = VectorSSE::CreateFromUint32(Constants::SIGN_BIT, Constants::SIGN_BIT, Constants::SIGN_BIT, Constants::SIGN_BIT);
	static const VectorSSE SignMask = VectorSSE::CreateFromUint32(~Constants::SIGN_BIT, ~Constants::SIGN_BIT, ~Constants::SIGN_BIT, ~Constants::SIGN_BIT);

	static const VectorSSE AllMask = VectorSSE::CreateFromUint32(0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF);

	/** Vector full of positive infinity */
	static const VectorSSE FloatInfinity = VectorSSE::CreateFromUint32(0x7F800000, 0x7F800000, 0x7F800000, 0x7F800000);

	static const VectorSSE Pi = VectorSSE(Constants::Pi);
	static const VectorSSE TwoPi = VectorSSE(2.0f * Constants::Pi);
	static const VectorSSE PiByTwo = VectorSSE(0.5f * Constants::Pi);
	static const VectorSSE PiByFour = VectorSSE(0.25f * Constants::Pi);
	static const VectorSSE OneOverPi = VectorSSE(1.0f / Constants::Pi);
	static const VectorSSE OneOverTwoPi = VectorSSE(1.0f / (2.0f * Constants::Pi));

	static const VectorSSE Float255 = VectorSSE(255.0f);
	static const VectorSSE Float360 = VectorSSE(360.f);
	static const VectorSSE Float180 = VectorSSE(180.f);
}

namespace GlobalVectorSSESin
{
	static const float p = 0.225f;
	static const float a = (16 * sqrt(p));
	static const float b = ((1 - p) / sqrt(p));
	static const VectorSSE A = VectorSSE(a, a, a, a);
	static const VectorSSE B = VectorSSE(b, b, b, b);
}




// shuffle requires constant expression which can not be encapsulated with functions
#define SHUFFLE_MASK(x, y, z, w)  ((x) | ((y) << 2) | ((z) << 4) | ((w) << 6))
#define SHUFFLE(Vec1, Vec2, x, y, z, w)   _mm_shuffle_ps((Vec1), (Vec2), SHUFFLE_MASK((x), (y), (z), (w)))



// implement forceinline
// i_array should be 16 bit aligned
FORCEINLINE VectorSSE::VectorSSE(const float *i_array)
	: XYZW(_mm_load_ps(i_array))
{
}

FORCEINLINE VectorSSE::VectorSSE(float i_x, float i_y, float i_z, float i_w)
	: XYZW(_mm_setr_ps(i_x, i_y, i_z, i_w))
{
}

FORCEINLINE VectorSSE::VectorSSE(float i_float)
	: XYZW(_mm_load_ps1(&i_float))
{
}

FORCEINLINE VectorSSE::VectorSSE(__m128 i_xyzw)
	: XYZW(i_xyzw)
{
}

FORCEINLINE VectorSSE::VectorSSE(const Vector2 & i_vector, float i_z, float i_w)
	: XYZW(_mm_setr_ps(i_vector.X, i_vector.Y, i_z, i_w))
{
}

FORCEINLINE VectorSSE::VectorSSE(const Vector3 & i_vector, float i_w)
	: XYZW(_mm_setr_ps(i_vector.X, i_vector.Y, i_vector.Z, i_w))
{
}

FORCEINLINE VectorSSE::VectorSSE(const Vector4 &i_vector)
	: XYZW(_mm_setr_ps(i_vector.X, i_vector.Y, i_vector.Z, i_vector.W))
{
}

FORCEINLINE VectorSSE::VectorSSE(const VectorSSE &i_other)
	: XYZW(i_other.XYZW)
{
}

FORCEINLINE VectorSSE& VectorSSE::operator=(const VectorSSE& i_other)
{
	XYZW = i_other.XYZW;
	return *this;
}


FORCEINLINE VectorSSE VectorSSE::CreateFromUint32(uint32 i_x, uint32 i_y, uint32 i_z, uint32 i_w)
{
	U32F32 x, y, z, w;
	x.asUint32 = i_x;
	y.asUint32 = i_y;
	z.asUint32 = i_z;
	w.asUint32 = i_w;
	return VectorSSE(x.asFloat32, y.asFloat32, z.asFloat32, w.asFloat32);
}


// o_array should be 16 bit aligned
FORCEINLINE void VectorSSE::GetFloatArray(float *o_array) const
{
	_mm_store_ps(o_array, XYZW);
}

// o_array don't need to be 16 bit aligned
FORCEINLINE void VectorSSE::GetFloatArrayUnAligned(float *o_array) const
{
	ALIGN(16) float res[4];
	_mm_store_ps(res, XYZW);
	memcpy(o_array, res, sizeof(float) * 4);
}



FORCEINLINE VectorSSE VectorSSE::Replicate(uint32 i_index) const
{
	switch (i_index)
	{
	case 0:
		return VectorSSE(SHUFFLE(XYZW, XYZW, 0, 0, 0, 0));
	case 1:
		return VectorSSE(SHUFFLE(XYZW, XYZW, 1, 1, 1, 1));
	case 2:
		return VectorSSE(SHUFFLE(XYZW, XYZW, 2, 2, 2, 2));
	case 3:
		return VectorSSE(SHUFFLE(XYZW, XYZW, 3, 3, 3, 3));
	default:
		ASSERT(0);
		return VectorSSE();
	}
}


FORCEINLINE VectorSSE VectorSSE::LengthSquare3() const
{
	return Dot3(*this, *this);
}

FORCEINLINE VectorSSE VectorSSE::LengthSquare4() const
{
	return Dot4(*this, *this);
}


FORCEINLINE VectorSSE VectorSSE::Length3() const
{
	return Dot3(*this, *this).Sqrt();
}
FORCEINLINE VectorSSE VectorSSE::Length4() const
{
	return Dot4(*this, *this).Sqrt();
}

// normalize the first 3, the last is illegal
FORCEINLINE VectorSSE& VectorSSE::Normalize3(float i_tolerance)
{
	VectorSSE sqrtDot3 = Dot3(*this, *this).Sqrt();
	ASSERT(sqrtDot3[0] > i_tolerance);
	return Divide(*this, sqrtDot3);
}

FORCEINLINE VectorSSE& VectorSSE::Normalize4(float i_tolerance)
{
	VectorSSE sqrtDot4 = Dot4(*this, *this).Sqrt();
	ASSERT(sqrtDot4[0] > i_tolerance);
	return Divide(*this, sqrtDot4);
}

FORCEINLINE VectorSSE VectorSSE::GetAbs() const
{
	return VectorSSE(_mm_and_ps(XYZW, VectorSSEConstants::SignMask.XYZW));
}

FORCEINLINE VectorSSE VectorSSE::Negate() const
{
	return VectorSSE(_mm_sub_ps(_mm_setzero_ps(), XYZW));
}

FORCEINLINE VectorSSE VectorSSE::Add(const VectorSSE &i_vector1, const VectorSSE &i_vector2)
{
	return VectorSSE(_mm_add_ps(i_vector1.XYZW, i_vector2.XYZW));
}
FORCEINLINE VectorSSE VectorSSE::Add(const VectorSSE &i_vector, float i_float)
{
	return VectorSSE(_mm_add_ps(i_vector.XYZW, _mm_set_ps(i_float, i_float, i_float, i_float)));
}

FORCEINLINE VectorSSE VectorSSE::Subtract(const VectorSSE &i_vector1, const VectorSSE &i_vector2)
{
	return VectorSSE(_mm_sub_ps(i_vector1.XYZW, i_vector2.XYZW));
}
FORCEINLINE VectorSSE VectorSSE::Subtract(const VectorSSE &i_vector, float i_float)
{
	return VectorSSE(_mm_sub_ps(i_vector.XYZW, _mm_set_ps(i_float, i_float, i_float, i_float)));
}

FORCEINLINE VectorSSE VectorSSE::Multiply(const VectorSSE &i_vector1, const VectorSSE &i_vector2)
{
	return VectorSSE(_mm_mul_ps(i_vector1.XYZW, i_vector2.XYZW));
}
FORCEINLINE VectorSSE VectorSSE::Multiply(const VectorSSE &i_vector, float i_float)
{
	return VectorSSE(_mm_mul_ps(i_vector.XYZW, _mm_set_ps(i_float, i_float, i_float, i_float)));
}

FORCEINLINE VectorSSE VectorSSE::Divide(const VectorSSE &i_vector1, const VectorSSE &i_vector2)
{
	return VectorSSE(_mm_div_ps(i_vector1.XYZW, i_vector2.XYZW));
}
FORCEINLINE VectorSSE VectorSSE::Divide(const VectorSSE &i_vector, float i_float)
{
	return VectorSSE(_mm_div_ps(i_vector.XYZW, _mm_set_ps(i_float, i_float, i_float, i_float)));
}

FORCEINLINE VectorSSE VectorSSE::MultiplyAdd(const VectorSSE &i_vector1, const VectorSSE &i_vector2, const VectorSSE &i_vector3)
{
	return VectorSSE(_mm_add_ps(_mm_mul_ps(i_vector1.XYZW, i_vector2.XYZW), i_vector3.XYZW));
}

FORCEINLINE VectorSSE VectorSSE::Dot3(const VectorSSE &i_vector1, const VectorSSE &i_vector2)
{
	VectorSSE temp = Multiply(i_vector1, i_vector2);
	return Add(temp.Replicate(0), Add(temp.Replicate(1), temp.Replicate(2)));
}

FORCEINLINE VectorSSE VectorSSE::Dot4(const VectorSSE &i_vector1, const VectorSSE &i_vector2)
{
	VectorSSE temp1, temp2;
	temp1 = Multiply(i_vector1, i_vector2);
	temp2 = VectorSSE(_mm_shuffle_ps(temp1.XYZW, temp1.XYZW, SHUFFLE_MASK(2, 3, 0, 1)));				// (Z,W,X,Y).
	temp1 = Add(temp1, temp2);																			// (X*X + Z*Z, Y*Y + W*W, Z*Z + X*X, W*W + Y*Y)
	temp2 = VectorSSE(_mm_shuffle_ps(temp1.XYZW, temp1.XYZW, SHUFFLE_MASK(1, 2, 3, 0)));				// Rotate left 4 bytes (Y,Z,W,X).
	return Add(temp1, temp2);																			// (X*X + Z*Z + Y*Y + W*W, Y*Y + W*W + Z*Z + X*X, Z*Z + X*X + W*W + Y*Y, W*W + Y*Y + X*X + Z*Z)
}

FORCEINLINE VectorSSE VectorSSE::Cross3(const VectorSSE &i_vector1, const VectorSSE &i_vector2)
{
	VectorSSE A_YZXW = VectorSSE(_mm_shuffle_ps(i_vector1.XYZW, i_vector1.XYZW, SHUFFLE_MASK(1, 2, 0, 3)));
	VectorSSE B_ZXYW = VectorSSE(_mm_shuffle_ps(i_vector2.XYZW, i_vector2.XYZW, SHUFFLE_MASK(2, 0, 1, 3)));
	VectorSSE A_ZXYW = VectorSSE(_mm_shuffle_ps(i_vector1.XYZW, i_vector1.XYZW, SHUFFLE_MASK(2, 0, 1, 3)));
	VectorSSE B_YZXW = VectorSSE(_mm_shuffle_ps(i_vector2.XYZW, i_vector2.XYZW, SHUFFLE_MASK(1, 2, 0, 3)));
	return Subtract(Multiply(A_YZXW, B_ZXYW), Multiply(A_ZXYW, B_YZXW));
}

FORCEINLINE VectorSSE VectorSSE::VectorSelect(const VectorSSE &i_mask, const VectorSSE &i_vector1, const VectorSSE &i_vector2)
{
	return VectorSSE(_mm_xor_ps(i_vector2.XYZW, _mm_and_ps(i_mask.XYZW, _mm_xor_ps(i_vector1.XYZW, i_vector2.XYZW))));
}

FORCEINLINE VectorSSE VectorSSE::BitwiseOr(const VectorSSE &i_vector1, const VectorSSE &i_vector2)
{
	return VectorSSE(_mm_or_ps(i_vector1.XYZW, i_vector2.XYZW));
}

FORCEINLINE VectorSSE VectorSSE::BitwiseAnd(const VectorSSE &i_vector1, const VectorSSE &i_vector2)
{
	return VectorSSE(_mm_and_ps(i_vector1.XYZW, i_vector2.XYZW));
}
FORCEINLINE VectorSSE VectorSSE::BitwiseXor(const VectorSSE &i_vector1, const VectorSSE &i_vector2)
{
	return VectorSSE(_mm_xor_ps(i_vector1.XYZW, i_vector2.XYZW));
}

FORCEINLINE VectorSSE VectorSSE::CompareEQ(const VectorSSE &i_vector1, const VectorSSE &i_vector2)
{
	return VectorSSE(_mm_cmpeq_ps(i_vector1.XYZW, i_vector2.XYZW));
}
FORCEINLINE VectorSSE VectorSSE::CompareNE(const VectorSSE &i_vector1, const VectorSSE &i_vector2)
{
	return VectorSSE(_mm_cmpneq_ps(i_vector1.XYZW, i_vector2.XYZW));
}
FORCEINLINE VectorSSE VectorSSE::CompareGT(const VectorSSE &i_vector1, const VectorSSE &i_vector2)
{
	return VectorSSE(_mm_cmpgt_ps(i_vector1.XYZW, i_vector2.XYZW));
}
FORCEINLINE VectorSSE VectorSSE::CompareGE(const VectorSSE &i_vector1, const VectorSSE &i_vector2)
{
	return VectorSSE(_mm_cmpge_ps(i_vector1.XYZW, i_vector2.XYZW));
}
FORCEINLINE VectorSSE VectorSSE::CompareLT(const VectorSSE &i_vector1, const VectorSSE &i_vector2)
{
	return VectorSSE(_mm_cmplt_ps(i_vector1.XYZW, i_vector2.XYZW));
}
FORCEINLINE VectorSSE VectorSSE::CompareLE(const VectorSSE &i_vector1, const VectorSSE &i_vector2)
{
	return VectorSSE(_mm_cmple_ps(i_vector1.XYZW, i_vector2.XYZW));
}

FORCEINLINE int32 VectorSSE::CompareAnyGT(const VectorSSE &i_vector1, const VectorSSE &i_vector2)
{
	return _mm_movemask_ps(_mm_cmpgt_ps(i_vector1.XYZW, i_vector2.XYZW));
}

FORCEINLINE int32 VectorSSE::MaskBits(const VectorSSE &i_vector)
{
	_mm_movemask_ps(i_vector.XYZW);
}

FORCEINLINE VectorSSE VectorSSE::Min(const VectorSSE &i_vector1, const VectorSSE &i_vector2)
{
	return VectorSSE(_mm_min_ps(i_vector1.XYZW, i_vector2.XYZW));
}

FORCEINLINE VectorSSE VectorSSE::Max(const VectorSSE &i_vector1, const VectorSSE &i_vector2)
{
	return VectorSSE(_mm_max_ps(i_vector1.XYZW, i_vector2.XYZW));
}

FORCEINLINE VectorSSE VectorSSE::Mod(const VectorSSE &i_vector1, const VectorSSE &i_vector2)
{
	VectorSSE temp = Divide(i_vector1, i_vector2).Truncate();
	return Subtract(i_vector1, Multiply(i_vector2, temp));
}

FORCEINLINE VectorSSE VectorSSE::Reciprocal() const
{
	return VectorSSE(_mm_rcp_ps(XYZW));
}

FORCEINLINE VectorSSE VectorSSE::ReciprocalLen(const VectorSSE& i_vector) const
{
	return Dot4(i_vector, i_vector).ReciprocalSqrt();
}

// from Unreal
// Return the reciprocal of the square root of each component
FORCEINLINE VectorSSE VectorSSE::ReciprocalSqrtAccurate() const
{
	// Perform two passes of Newton-Raphson iteration on the hardware estimate
	//    v^-0.5 = x
	// => x^2 = v^-1
	// => 1/(x^2) = v
	// => F(x) = x^-2 - v
	//    F'(x) = -2x^-3

	//    x1 = x0 - F(x0)/F'(x0)
	// => x1 = x0 + 0.5 * (x0^-2 - Vec) * x0^3
	// => x1 = x0 + 0.5 * (x0 - Vec * x0^3)
	// => x1 = x0 + x0 * (0.5 - 0.5 * Vec * x0^2)

	const VectorSSE VecDivBy2 = Multiply(*this, VectorSSEConstants::FloatOneHalf);

	// Initial estimate
	const VectorSSE x0 = Reciprocal();

	// First iteration
	VectorSSE x1 = Multiply(x0, x0);
	x1 = Subtract(VectorSSEConstants::FloatOneHalf, Multiply(VecDivBy2, x1));
	x1 = MultiplyAdd(x0, x1, x0);

	// Second iteration
	VectorSSE x2 = Multiply(x1, x1);
	x2 = Subtract(VectorSSEConstants::FloatOneHalf, Multiply(VecDivBy2, x2));
	x2 = MultiplyAdd(x1, x2, x1);

	return x2;
}

// Computes the reciprocal of a vector(component - wise) and returns the result.
FORCEINLINE VectorSSE VectorSSE::VectorReciprocalAccurate() const
{
	// Perform two passes of Newton-Raphson iteration on the hardware estimate
	//   x1 = x0 - f(x0) / f'(x0)
	//
	//    1 / Vec = x
	// => x * Vec = 1 
	// => F(x) = x * Vec - 1
	//    F'(x) = Vec
	// => x1 = x0 - (x0 * Vec - 1) / Vec
	//
	// Since 1/Vec is what we're trying to solve, use an estimate for it, x0
	// => x1 = x0 - (x0 * Vec - 1) * x0 = 2 * x0 - Vec * x0^2 

	// Initial estimate
	const VectorSSE x0 = Reciprocal();
	// First iteration
	const VectorSSE x1 = Subtract(Add(x0, x0), Multiply(*this, Multiply(x0, x0)));
	// Second iteration
	const VectorSSE x2 = Subtract(Add(x1, x1), Multiply(*this, Multiply(x1, x1)));

	return x2;
}

FORCEINLINE VectorSSE VectorSSE::Sqrt() const
{
	return VectorSSE(_mm_sqrt_ps(XYZW));
}

FORCEINLINE VectorSSE VectorSSE::ReciprocalSqrt() const
{
	return VectorSSE(_mm_rsqrt_ps(XYZW));
}

FORCEINLINE VectorSSE VectorSSE::Truncate() const
{
	return VectorSSE(_mm_cvtepi32_ps(_mm_cvttps_epi32(XYZW)));
}

FORCEINLINE VectorSSE VectorSSE::Fractional() const
{
	return VectorSSE(Subtract(*this, Truncate()));
}

FORCEINLINE VectorSSE VectorSSE::Ceil() const
{
	VectorSSE trunc = Truncate();
	VectorSSE posMask = CompareGE(*this, VectorSSEConstants::FloatZero);
	VectorSSE add = VectorSelect(posMask, VectorSSEConstants::FloatOne, VectorSSEConstants::FloatZero);
	return Add(trunc, add);
}
FORCEINLINE VectorSSE VectorSSE::Floor() const
{
	VectorSSE trunc = Truncate();
	VectorSSE posMask = CompareGE(*this, VectorSSEConstants::FloatZero);
	VectorSSE sub = VectorSelect(posMask, VectorSSEConstants::FloatZero, VectorSSEConstants::FloatOne);
	return Subtract(trunc, sub);
}


FORCEINLINE float VectorSSE::operator[](uint32 i_index) const
{
	ASSERT(i_index < 4);
	return (reinterpret_cast<const float*>(&(XYZW))[i_index]);
}


// for vector2, 3 and 4

FORCEINLINE Vector2::Vector2(const VectorSSE &i_vectorSSE)
	: X(i_vectorSSE[0]), Y(i_vectorSSE[1])
{
}

FORCEINLINE Vector3::Vector3(const VectorSSE &i_vectorSSE)
	: X(i_vectorSSE[0]), Y(i_vectorSSE[1]), Z(i_vectorSSE[2])
{	
}

FORCEINLINE Vector4::Vector4(const VectorSSE &i_vectorSSE)
{
	i_vectorSSE.GetFloatArrayUnAligned(&X);
}


//for math, from Unreal

FORCEINLINE VectorSSE VectorSSE::VectorSin()
{
	//Sine approximation using a squared parabola restrained to f(0) = 0, f(PI) = 0, f(PI/2) = 1.
	//based on a good discussion here http://forum.devmaster.net/t/fast-and-accurate-sine-cosine/9648
	//After approx 2.5 million tests comparing to sin(): 
	//Average error of 0.000128
	//Max error of 0.001091

	VectorSSE y = Multiply(*this, VectorSSEConstants::OneOverTwoPi);
	y = Subtract(y, Add(y, VectorSSEConstants::FloatOneHalf).Floor());
	y = Multiply(GlobalVectorSSESin::A, Multiply(y, Subtract(VectorSSEConstants::FloatOneHalf, y.GetAbs())));
	return Multiply(y, Add(GlobalVectorSSESin::B, y.GetAbs()));
}

FORCEINLINE VectorSSE VectorSSE::VectorCos()
{
	return Add(*this, VectorSSEConstants::PiByTwo).VectorSin();
}

FORCEINLINE void Math::VectorSinCos(VectorSSE* o_sinAngles, VectorSSE* o_cosAngles, const VectorSSE* i_angles)
{
	// Map to [-pi, pi]
	// X = A - 2pi * round(A/2pi)
	// Note the round(), not truncate(). In this case round() can round halfway cases using round-to-nearest-even OR round-to-nearest.

	// Quotient = round(A/2pi)
	VectorSSE quotient = VectorSSE::Multiply(*i_angles, VectorSSEConstants::OneOverTwoPi);
	quotient = VectorSSE(_mm_cvtepi32_ps(_mm_cvtps_epi32(quotient.XYZW))); // round to nearest even is the default rounding mode but that's fine here.
														   // X = A - 2pi * Quotient
	VectorSSE X = VectorSSE::Subtract(*i_angles, VectorSSE::Multiply(VectorSSEConstants::TwoPi, quotient));

	// Map in [-pi/2,pi/2]
	VectorSSE sign = VectorSSE::BitwiseAnd(X, VectorSSEConstants::SignBit);
	VectorSSE c = VectorSSE::BitwiseOr(VectorSSEConstants::Pi, sign);  // pi when x >= 0, -pi when x < 0
	VectorSSE absx = X.GetAbs();
	VectorSSE rflx = VectorSSE::Subtract(c, X);
	VectorSSE comp = VectorSSE::CompareGT(absx, VectorSSEConstants::PiByTwo);
	X = VectorSSE::VectorSelect(comp, rflx, X);
	sign = VectorSSE::VectorSelect(comp, VectorSSEConstants::FloatMinusOne, VectorSSEConstants::FloatOne);

	const VectorSSE XSquared = VectorSSE::Multiply(X, X);

	// 11-degree minimax approximation
	//*ScalarSin = (((((-2.3889859e-08f * y2 + 2.7525562e-06f) * y2 - 0.00019840874f) * y2 + 0.0083333310f) * y2 - 0.16666667f) * y2 + 1.0f) * y;
	const VectorSSE SinCoeff0 = VectorSSE(1.0f, -0.16666667f, 0.0083333310f, -0.00019840874f);
	const VectorSSE SinCoeff1 = VectorSSE(2.7525562e-06f, -2.3889859e-08f, /*unused*/ 0.f, /*unused*/ 0.f);

	VectorSSE S;
	S = SinCoeff1.Replicate(1);
	S = VectorSSE::MultiplyAdd(XSquared, S, SinCoeff1.Replicate(0));
	S = VectorSSE::MultiplyAdd(XSquared, S, SinCoeff0.Replicate(3));
	S = VectorSSE::MultiplyAdd(XSquared, S, SinCoeff0.Replicate(2));
	S = VectorSSE::MultiplyAdd(XSquared, S, SinCoeff0.Replicate(1));
	S = VectorSSE::MultiplyAdd(XSquared, S, SinCoeff0.Replicate(0));
	*o_sinAngles = VectorSSE::Multiply(S, X);

	// 10-degree minimax approximation
	//*ScalarCos = sign * (((((-2.6051615e-07f * y2 + 2.4760495e-05f) * y2 - 0.0013888378f) * y2 + 0.041666638f) * y2 - 0.5f) * y2 + 1.0f);
	const VectorSSE CosCoeff0 = VectorSSE(1.0f, -0.5f, 0.041666638f, -0.0013888378f);
	const VectorSSE CosCoeff1 = VectorSSE(2.4760495e-05f, -2.6051615e-07f, /*unused*/ 0.f, /*unused*/ 0.f);

	VectorSSE C;
	C = CosCoeff1.Replicate(1);
	C = VectorSSE::MultiplyAdd(XSquared, C, CosCoeff1.Replicate(0));
	C = VectorSSE::MultiplyAdd(XSquared, C, CosCoeff0.Replicate(3));
	C = VectorSSE::MultiplyAdd(XSquared, C, CosCoeff0.Replicate(2));
	C = VectorSSE::MultiplyAdd(XSquared, C, CosCoeff0.Replicate(1));
	C = VectorSSE::MultiplyAdd(XSquared, C, CosCoeff0.Replicate(0));
	*o_cosAngles = VectorSSE::Multiply(C, sign);
}
