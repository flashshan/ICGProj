#pragma once

#include "VectorSSE.h"
#include "Axis.h"

#include "Core\Math\Rotator.h"

struct Quaternion;

ALIGN(16) struct Matrix {
public:
	FORCEINLINE Matrix();
	FORCEINLINE Matrix(float i_11, float i_12, float i_13, float i_14,
		float i_21, float i_22, float i_23, float i_24,
		float i_31, float i_32, float i_33, float i_34,
		float i_41, float i_42, float i_43, float i_44);
	explicit FORCEINLINE Matrix(const float *i_floatArray);
	FORCEINLINE Matrix(const Vector4 &i_v1, const Vector4 &i_v2, const Vector4 &i_v3, const Vector4 &i_v4);
	FORCEINLINE Matrix(const VectorSSE &i_v1, const VectorSSE &i_v2, const VectorSSE &i_v3, const VectorSSE &i_v4);
	explicit Matrix(const Rotator& i_rotator);

	FORCEINLINE static Matrix CreateZero();
	FORCEINLINE static Matrix CreateIdentity();

	FORCEINLINE static Matrix CreateTranslate(float i_xTranslate, float i_yTranslate, float i_zTranslate);
	FORCEINLINE static Matrix CreateTranslate(const Vector3& i_Vector);
	FORCEINLINE static Matrix CreateXRotation(float i_XRadian);
	FORCEINLINE static Matrix CreateYRotation(float i_YRadian);
	FORCEINLINE static Matrix CreateZRotation(float i_ZRadian);
	FORCEINLINE static Matrix CreateScale(float i_xScale, float i_yScale, float i_zScale);
	FORCEINLINE static Matrix CreateScale(const Vector3& i_scale);

	FORCEINLINE bool IsSingular() const;
	FORCEINLINE VectorSSE GetRow(uint32 i_index) const;
	FORCEINLINE VectorSSE GetCol(uint32 i_index) const;

	Matrix& Transpose();
	FORCEINLINE Matrix GetTranspose() const;

	Matrix& Inverse();
	Matrix GetInverse() const;

	Matrix operator*(float i_float) const;
	Matrix& operator*=(float i_float);
	Matrix operator/(float i_float) const;
	Matrix& operator/=(float i_float);
	Matrix operator*(const Matrix &i_other) const;
	Matrix& operator*=(const Matrix &i_other);

	FORCEINLINE float operator [](uint32 i_index) const;
	
	inline Vector3 GetScaledAxis(EAxis i_axis) const;
	FORCEINLINE Quaternion ToQuaternion() const;

	Vector4 MultiplyLeft(const Vector4 &i_vector) const;
	Vector4 MultiplyRight(const Vector4 &i_vector) const;

	VectorSSE Matrix::MultiplyLeft(const VectorSSE &i_vector) const;
	VectorSSE Matrix::MultiplyRight(const VectorSSE &i_vector) const;

public:
	static Matrix FastMatrix;

public:
	float M[4][4];
};





// implement forceinline
// for debug, no default matrix should be used
FORCEINLINE Matrix::Matrix()
{
	M[0][0] = NAN;
}

FORCEINLINE Matrix::Matrix(float i_11, float i_12, float i_13, float i_14,
	float i_21, float i_22, float i_23, float i_24,
	float i_31, float i_32, float i_33, float i_34,
	float i_41, float i_42, float i_43, float i_44)
	: M{ i_11, i_12, i_13, i_14, i_21, i_22, i_23, i_24, i_31, i_32, i_33, i_34, i_41, i_42, i_43, i_44 }
{
	ASSERT(!Float::IsNAN(M[0][0]));
}

FORCEINLINE Matrix::Matrix(const float *i_floatArray)
	: M{ i_floatArray[0], i_floatArray[1], i_floatArray[2], i_floatArray[3],
		 i_floatArray[4], i_floatArray[5], i_floatArray[6], i_floatArray[7], 
		 i_floatArray[8], i_floatArray[9], i_floatArray[10], i_floatArray[11], 
		 i_floatArray[12], i_floatArray[13], i_floatArray[14], i_floatArray[15] }
{
	ASSERT(!Float::IsNAN(M[0][0]));
}

FORCEINLINE Matrix::Matrix(const Vector4 &i_v1, const Vector4 &i_v2, const Vector4 &i_v3, const Vector4 &i_v4)
	: M{ i_v1.X, i_v1.Y, i_v1.Z, i_v1.W,
		 i_v2.X, i_v2.Y, i_v2.Z, i_v2.W,
		 i_v3.X, i_v3.Y, i_v3.Z, i_v3.W,
		 i_v4.X, i_v4.Y, i_v4.Z, i_v4.W }
{
	ASSERT(!Float::IsNAN(M[0][0]));
}

FORCEINLINE Matrix::Matrix(const VectorSSE &i_v1, const VectorSSE &i_v2, const VectorSSE &i_v3, const VectorSSE &i_v4)
	: M{ i_v1[0], i_v1[1], i_v1[2], i_v1[3],
		 i_v2[0], i_v2[1], i_v2[2], i_v2[3],
		 i_v3[0], i_v3[1], i_v3[2], i_v3[3],
		 i_v4[0], i_v4[1], i_v4[2], i_v4[3] }
{
}


FORCEINLINE Matrix Matrix::CreateZero()
{
	return Matrix(0.0f, 0.0f, 0.0f, 0.0f,
				  0.0f, 0.0f, 0.0f, 0.0f,
				  0.0f, 0.0f, 0.0f, 0.0f,
				  0.0f, 0.0f, 0.0f, 0.0f);
}

FORCEINLINE Matrix Matrix::CreateIdentity()
{
	return Matrix(1.0f, 0.0f, 0.0f, 0.0f,
				  0.0f, 1.0f, 0.0f, 0.0f,
				  0.0f, 0.0f, 1.0f, 0.0f,
				  0.0f, 0.0f, 0.0f, 1.0f);
}

FORCEINLINE Matrix Matrix::CreateTranslate(float i_xTranslate, float i_yTranslate, float i_zTranslate)
{
	return Matrix(1.0f, 0.0f, 0.0f, 0.0f,
				  0.0f, 1.0f, 0.0f, 0.0f,
				  0.0f, 0.0f, 1.0f, 0.0f,
				  i_xTranslate, i_yTranslate, i_zTranslate, 1.0f);
}

FORCEINLINE Matrix Matrix::CreateTranslate(const Vector3& i_Vector)
{
	return Matrix(1.0f, 0.0f, 0.0f, 0.0f,
				  0.0f, 1.0f, 0.0f, 0.0f,
				  0.0f, 0.0f, 1.0f, 0.0f,
				  i_Vector.X, i_Vector.Y, i_Vector.Z, 1.0f);
}

FORCEINLINE Matrix Matrix::CreateXRotation(float i_XRadian)
{
	float sin, cos;
	Math::SinCos(&sin, &cos, i_XRadian);
	return Matrix(1.0f, 0.0f, 0.0f, 0.0f,
				  0.0f, cos, sin, 0.0f,
				  0.0f, -sin, cos, 0.0f,
				  0.0f, 0.0f, 0.0f, 1.0f);
}

FORCEINLINE Matrix Matrix::CreateYRotation(float i_YRadian)
{
	float sin, cos;
	Math::SinCos(&sin, &cos, i_YRadian);
	return Matrix(cos, 0.0f, sin, 0.0f,
				  0.0f, 1.0f, 0.0f, 0.0f,
				  -sin, 0.0f, cos, 0.0f,
				  0.0f, 0.0f, 0.0f, 1.0f);
}

FORCEINLINE Matrix Matrix::CreateZRotation(float i_ZRadian)
{
	float sin, cos;
	Math::SinCos(&sin, &cos, i_ZRadian);
	return Matrix(cos, sin, 0.0f, 0.0f,
				  -sin, cos, 0.0f, 0.0f,
				  0.0f, 0.0f, 1.0f, 0.0f,
				  0.0f, 0.0f, 0.0f, 1.0f);
}

FORCEINLINE Matrix Matrix::CreateScale(float i_xScale, float i_yScale, float i_zScale)
{
	return Matrix(i_xScale, 0.0f, 0.0f, 0.0f,
				  0.0f, i_yScale, 0.0f, 0.0f,
				  0.0f, 0.0f, i_zScale, 0.0f,
				  0.0f, 0.0f, 0.0f, 1.0f);
}

FORCEINLINE Matrix Matrix::CreateScale(const Vector3& i_scale)
{
	return Matrix(i_scale.X, 0.0f, 0.0f, 0.0f,
				  0.0f, i_scale.Y, 0.0f, 0.0f,
				  0.0f, 0.0f, i_scale.Z, 0.0f,
				  0.0f, 0.0f, 0.0f, 1.0f);
}

FORCEINLINE bool Matrix::IsSingular() const
{
	ASSERT(!Float::IsNAN(M[0][0]));
	return (Float::IsZero((M[0][0] * M[0][0] + M[0][1] * M[0][1] + M[0][2] * M[0][2] + M[0][3] * M[0][3])) ||
			Float::IsZero((M[1][0] * M[1][0] + M[1][1] * M[1][1] + M[1][2] * M[1][2] + M[1][3] * M[1][3])) ||
			Float::IsZero((M[2][0] * M[2][0] + M[2][1] * M[2][1] + M[2][2] * M[2][2] + M[2][3] * M[2][3])) ||
			Float::IsZero((M[3][0] * M[3][0] + M[3][1] * M[3][1] + M[3][2] * M[3][2] + M[3][3] * M[3][3])));
}

FORCEINLINE Matrix Matrix::GetTranspose() const
{
	ASSERT(!Float::IsNAN(M[0][0]));
	return Matrix(M[0][0], M[1][0], M[2][0], M[3][0],
				  M[0][1], M[1][1], M[2][1], M[3][1],
				  M[0][2], M[1][2], M[2][2], M[3][2],
				  M[0][3], M[1][3], M[2][3], M[3][3]);
}

FORCEINLINE VectorSSE Matrix::GetRow(uint32 i_index) const
{
	return VectorSSE(M[i_index][0], M[i_index][1], M[i_index][2], M[i_index][3]);
}

FORCEINLINE VectorSSE Matrix::GetCol(uint32 i_index) const
{
	return VectorSSE(M[0][i_index], M[1][i_index], M[2][i_index], M[3][i_index]);
}

FORCEINLINE float Matrix::operator [](uint32 i_index) const
{
	ASSERT(i_index < 16);
	return M[i_index / 4][i_index % 4];
}

inline Vector3 Matrix::GetScaledAxis(EAxis i_axis) const
{
	switch (i_axis)
	{
	case EAxis::X:
		return Vector3(M[0][0], M[0][1], M[0][2]);
	case EAxis::Y:
		return Vector3(M[1][0], M[1][1], M[1][2]);
	case EAxis::Z:
		return Vector3(M[2][0], M[2][1], M[2][2]);
	default:
		ASSERT(0);
		return Vector3::Zero();
	}
}


// for Vector4

FORCEINLINE Vector4 Vector4::Mul(const Matrix &i_matrix) const
{
	ASSERT(!Float::IsNAN(i_matrix.M[0][0]));
	// default mul is vector left, matrix right
	return i_matrix.MultiplyLeft(*this);
}

// for VectorSSE
FORCEINLINE VectorSSE VectorSSE::Mul(const Matrix &i_matrix) const
{
	ASSERT(!Float::IsNAN(i_matrix.M[0][0]));
	return i_matrix.MultiplyLeft(*this);
}
