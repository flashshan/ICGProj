#include "Core\Math\Matrix.h"

#include <xmmintrin.h>
#include "Core/Basic/BasicFunctions.h"

static Matrix FastMatrix(
	1.0f, 0.0f, 0.0f, 0.0f,
	0.0f, 1.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 1.0f, 0.0f,
	0.0f, 0.0f, 0.0f, 1.0f);



Matrix::Matrix(const Rotator& i_rotator)
{
	const VectorSSE Angles(i_rotator.Roll, i_rotator.Yaw, i_rotator.Pitch, 0.0f);
	const VectorSSE HalfAngles = VectorSSE::Multiply(Angles, VectorSSEConstants::DEG_TO_RAD);


	VectorSSE SinAngles, CosAngles;
	Math::VectorSinCos(&SinAngles, &CosAngles, &HalfAngles);

	const float	SP = SinAngles[0];
	const float	SY = SinAngles[1];
	const float	SR = SinAngles[2];
	const float	CP = CosAngles[0];
	const float	CY = CosAngles[1];
	const float	CR = CosAngles[2];

	M[0][0] = CP * CY,
	M[0][1] = CP * SY;
	M[0][2] = SP;
	M[0][3] = 0.f;

	M[1][0] = SR * SP * CY - CR * SY;
	M[1][1] = SR * SP * SY + CR * CY;
	M[1][2] = -SR * CP;
	M[1][3] = 0.f;

	M[2][0] = -(CR * SP * CY + SR * SY);
	M[2][1] = CY * SR - CR * SP * SY;
	M[2][2] = CR * CP;
	M[2][3] = 0.f;

	M[3][0] = 0.0f;
	M[3][1] = 0.0f;
	M[3][2] = 0.0f;
	M[3][3] = 1.f;
}


Matrix& Matrix::Transpose()
{
	float temp;
	temp = M[0][1]; M[0][1] = M[1][0]; M[1][0] = temp;
	temp = M[0][2]; M[0][2] = M[2][0]; M[2][0] = temp;
	temp = M[0][3]; M[0][3] = M[3][0]; M[3][0] = temp;
	temp = M[1][2]; M[1][2] = M[2][1]; M[2][1] = temp;
	temp = M[1][3]; M[1][3] = M[3][1]; M[3][1] = temp;
	temp = M[2][3]; M[2][3] = M[3][2]; M[3][2] = temp;
	return *this;
}

//fast inverse from Unreal
static void Inverse4x4(float *o_dst, const float *i_src)
{
	typedef float Float4x4[4][4];
	const Float4x4& M = *((const Float4x4*)i_src);
	Float4x4 result;
	float det[4];
	Float4x4 temp;

	temp[0][0] = M[2][2] * M[3][3] - M[2][3] * M[3][2];
	temp[0][1] = M[1][2] * M[3][3] - M[1][3] * M[3][2];
	temp[0][2] = M[1][2] * M[2][3] - M[1][3] * M[2][2];

	temp[1][0] = M[2][2] * M[3][3] - M[2][3] * M[3][2];
	temp[1][1] = M[0][2] * M[3][3] - M[0][3] * M[3][2];
	temp[1][2] = M[0][2] * M[2][3] - M[0][3] * M[2][2];

	temp[2][0] = M[1][2] * M[3][3] - M[1][3] * M[3][2];
	temp[2][1] = M[0][2] * M[3][3] - M[0][3] * M[3][2];
	temp[2][2] = M[0][2] * M[1][3] - M[0][3] * M[1][2];

	temp[3][0] = M[1][2] * M[2][3] - M[1][3] * M[2][2];
	temp[3][1] = M[0][2] * M[2][3] - M[0][3] * M[2][2];
	temp[3][2] = M[0][2] * M[1][3] - M[0][3] * M[1][2];

	det[0] = M[1][1] * temp[0][0] - M[2][1] * temp[0][1] + M[3][1] * temp[0][2];
	det[1] = M[0][1] * temp[1][0] - M[2][1] * temp[1][1] + M[3][1] * temp[1][2];
	det[2] = M[0][1] * temp[2][0] - M[1][1] * temp[2][1] + M[3][1] * temp[2][2];
	det[3] = M[0][1] * temp[3][0] - M[1][1] * temp[3][1] + M[2][1] * temp[3][2];

	float determinant = M[0][0] * det[0] - M[1][0] * det[1] + M[2][0] * det[2] - M[3][0] * det[3];
	const float	rDet = 1.0f / determinant;

	result[0][0] = rDet * det[0];
	result[0][1] = -rDet * det[1];
	result[0][2] = rDet * det[2];
	result[0][3] = -rDet * det[3];
	result[1][0] = -rDet * (M[1][0] * temp[0][0] - M[2][0] * temp[0][1] + M[3][0] * temp[0][2]);
	result[1][1] = rDet * (M[0][0] * temp[1][0] - M[2][0] * temp[1][1] + M[3][0] * temp[1][2]);
	result[1][2] = -rDet * (M[0][0] * temp[2][0] - M[1][0] * temp[2][1] + M[3][0] * temp[2][2]);
	result[1][3] = rDet * (M[0][0] * temp[3][0] - M[1][0] * temp[3][1] + M[2][0] * temp[3][2]);
	result[2][0] = rDet * (
		M[1][0] * (M[2][1] * M[3][3] - M[2][3] * M[3][1]) -
		M[2][0] * (M[1][1] * M[3][3] - M[1][3] * M[3][1]) +
		M[3][0] * (M[1][1] * M[2][3] - M[1][3] * M[2][1])
		);
	result[2][1] = -rDet * (
		M[0][0] * (M[2][1] * M[3][3] - M[2][3] * M[3][1]) -
		M[2][0] * (M[0][1] * M[3][3] - M[0][3] * M[3][1]) +
		M[3][0] * (M[0][1] * M[2][3] - M[0][3] * M[2][1])
		);
	result[2][2] = rDet * (
		M[0][0] * (M[1][1] * M[3][3] - M[1][3] * M[3][1]) -
		M[1][0] * (M[0][1] * M[3][3] - M[0][3] * M[3][1]) +
		M[3][0] * (M[0][1] * M[1][3] - M[0][3] * M[1][1])
		);
	result[2][3] = -rDet * (
		M[0][0] * (M[1][1] * M[2][3] - M[1][3] * M[2][1]) -
		M[1][0] * (M[0][1] * M[2][3] - M[0][3] * M[2][1]) +
		M[2][0] * (M[0][1] * M[1][3] - M[0][3] * M[1][1])
		);
	result[3][0] = -rDet * (
		M[1][0] * (M[2][1] * M[3][2] - M[2][2] * M[3][1]) -
		M[2][0] * (M[1][1] * M[3][2] - M[1][2] * M[3][1]) +
		M[3][0] * (M[1][1] * M[2][2] - M[1][2] * M[2][1])
		);
	result[3][1] = rDet * (
		M[0][0] * (M[2][1] * M[3][2] - M[2][2] * M[3][1]) -
		M[2][0] * (M[0][1] * M[3][2] - M[0][2] * M[3][1]) +
		M[3][0] * (M[0][1] * M[2][2] - M[0][2] * M[2][1])
		);
	result[3][2] = -rDet * (
		M[0][0] * (M[1][1] * M[3][2] - M[1][2] * M[3][1]) -
		M[1][0] * (M[0][1] * M[3][2] - M[0][2] * M[3][1]) +
		M[3][0] * (M[0][1] * M[1][2] - M[0][2] * M[1][1])
		);
	result[3][3] = rDet * (
		M[0][0] * (M[1][1] * M[2][2] - M[1][2] * M[2][1]) -
		M[1][0] * (M[0][1] * M[2][2] - M[0][2] * M[2][1]) +
		M[2][0] * (M[0][1] * M[1][2] - M[0][2] * M[1][1])
		);

	memcpy(o_dst, &result, 16 * sizeof(float));
}
/* another version of inverse
const double s0 = static_cast<double>(i_src[0]);   const double s1 = static_cast<double>(i_src[1]);   const double s2 = static_cast<double>(i_src[2]);   const double s3 = static_cast<double>(i_src[3]);
const double s4 = static_cast<double>(i_src[4]);   const double s5 = static_cast<double>(i_src[5]);   const double s6 = static_cast<double>(i_src[6]);   const double s7 = static_cast<double>(i_src[7]);
const double s8 = static_cast<double>(i_src[8]);   const double s9 = static_cast<double>(i_src[9]);   const double s10 = static_cast<double>(i_src[10]); const double s11 = static_cast<double>(i_src[11]);
const double s12 = static_cast<double>(i_src[12]); const double s13 = static_cast<double>(i_src[13]); const double s14 = static_cast<double>(i_src[14]); const double s15 = static_cast<double>(i_src[15]);

double inv[16];
inv[0] = s5 * s10 * s15 - s5 * s11 * s14 - s9 * s6 * s15 + s9 * s7 * s14 + s13 * s6 * s11 - s13 * s7 * s10;
inv[1] = -s1 * s10 * s15 + s1 * s11 * s14 + s9 * s2 * s15 - s9 * s3 * s14 - s13 * s2 * s11 + s13 * s3 * s10;
inv[2] = s1 * s6  * s15 - s1 * s7  * s14 - s5 * s2 * s15 + s5 * s3 * s14 + s13 * s2 * s7 - s13 * s3 * s6;
inv[3] = -s1 * s6  * s11 + s1 * s7  * s10 + s5 * s2 * s11 - s5 * s3 * s10 - s9  * s2 * s7 + s9  * s3 * s6;
inv[4] = -s4 * s10 * s15 + s4 * s11 * s14 + s8 * s6 * s15 - s8 * s7 * s14 - s12 * s6 * s11 + s12 * s7 * s10;
inv[5] = s0 * s10 * s15 - s0 * s11 * s14 - s8 * s2 * s15 + s8 * s3 * s14 + s12 * s2 * s11 - s12 * s3 * s10;
inv[6] = -s0 * s6  * s15 + s0 * s7  * s14 + s4 * s2 * s15 - s4 * s3 * s14 - s12 * s2 * s7 + s12 * s3 * s6;
inv[7] = s0 * s6  * s11 - s0 * s7  * s10 - s4 * s2 * s11 + s4 * s3 * s10 + s8  * s2 * s7 - s8  * s3 * s6;
inv[8] = s4 * s9  * s15 - s4 * s11 * s13 - s8 * s5 * s15 + s8 * s7 * s13 + s12 * s5 * s11 - s12 * s7 * s9;
inv[9] = -s0 * s9  * s15 + s0 * s11 * s13 + s8 * s1 * s15 - s8 * s3 * s13 - s12 * s1 * s11 + s12 * s3 * s9;
inv[10] = s0 * s5  * s15 - s0 * s7  * s13 - s4 * s1 * s15 + s4 * s3 * s13 + s12 * s1 * s7 - s12 * s3 * s5;
inv[11] = -s0 * s5  * s11 + s0 * s7  * s9 + s4 * s1 * s11 - s4 * s3 * s9 - s8  * s1 * s7 + s8  * s3 * s5;
inv[12] = -s4 * s9  * s14 + s4 * s10 * s13 + s8 * s5 * s14 - s8 * s6 * s13 - s12 * s5 * s10 + s12 * s6 * s9;
inv[13] = s0 * s9  * s14 - s0 * s10 * s13 - s8 * s1 * s14 + s8 * s2 * s13 + s12 * s1 * s10 - s12 * s2 * s9;
inv[14] = -s0 * s5  * s14 + s0 * s6  * s13 + s4 * s1 * s14 - s4 * s2 * s13 - s12 * s1 * s6 + s12 * s2 * s5;
inv[15] = s0 * s5  * s10 - s0 * s6  * s9 - s4 * s1 * s10 + s4 * s2 * s9 + s8  * s1 * s6 - s8  * s2 * s5;

double det = s0 * inv[0] + s1 * inv[4] + s2 * inv[8] + s3 * inv[12];
if (det != 0.0)
{
det = 1.0 / det;
}
for (int i = 0; i < 16; ++i)
{
o_dst[i] = static_cast<float>(inv[i] * det);
}*/


Matrix& Matrix::Inverse()
{
	if (IsSingular())
	{
		DEBUG_PRINT("Inverse Singular Matrix!\n");
		for (int i = 0; i < 16; ++i)
		{
			if (i / 4 == i % 4) M[i / 4][i % 4] = 1;
			else M[i / 4][i % 4] = 0;
		}
	}
	else
	{
		float res[16];
		Inverse4x4(res, M[0]);
		for (int i = 0; i < 16; ++i)
		{
			M[i / 4][i % 4] = res[i];
		}
	}
	return *this;
}


Matrix Matrix::GetInverse() const
{
	if (IsSingular())
	{
		DEBUG_PRINT("Inverse Singular Matrix!\n");
		return Matrix::CreateIdentity();
	}

	float res[16];
	Inverse4x4(res, M[0]);
	return Matrix(res);
}

Matrix Matrix::operator*(float i_float) const
{
	return Matrix(M[0][0] * i_float, M[0][1] * i_float, M[0][2] * i_float, M[0][3] * i_float,
		M[1][0] * i_float, M[1][1] * i_float, M[1][2] * i_float, M[1][3] * i_float,
		M[2][0] * i_float, M[2][1] * i_float, M[2][2] * i_float, M[2][3] * i_float,
		M[3][0] * i_float, M[3][1] * i_float, M[3][2] * i_float, M[3][3] * i_float);
}

Matrix& Matrix::operator*=(float i_float)
{
	for (int i = 0; i < 16; ++i)
	{
		M[i / 4][i % 4] *= i_float;
	}
	return *this;
}

Matrix Matrix::operator/(float i_float) const
{
	const float reciprocal = 1.0f / i_float;
	return Matrix(M[0][0] * reciprocal, M[0][1] * reciprocal, M[0][2] * reciprocal, M[0][3] * reciprocal,
		M[1][0] * reciprocal, M[1][1] * reciprocal, M[1][2] * reciprocal, M[1][3] * reciprocal,
		M[2][0] * reciprocal, M[2][1] * reciprocal, M[2][2] * reciprocal, M[2][3] * reciprocal,
		M[3][0] * reciprocal, M[3][1] * reciprocal, M[3][2] * reciprocal, M[3][3] * reciprocal);
}

Matrix& Matrix::operator/=(float i_float)
{
	const float reciprocal = 1.0f / i_float;
	for (int i = 0; i < 16; ++i)
	{
		M[i / 4][i % 4] *= reciprocal;
	}
	return *this;
}

Matrix Matrix::operator*(const Matrix &i_other) const
{
	const VectorSSE *matrix1 = reinterpret_cast<const VectorSSE *>(this);
	const VectorSSE *matrix2 = reinterpret_cast<const VectorSSE *>(&i_other);

	VectorSSE temp;
	temp = VectorSSE::Multiply(matrix1[0].Replicate(0), matrix2[0]);
	temp = VectorSSE::MultiplyAdd(matrix1[0].Replicate(1), matrix2[1], temp);
	temp = VectorSSE::MultiplyAdd(matrix1[0].Replicate(2), matrix2[2], temp);
	VectorSSE r0 = VectorSSE::MultiplyAdd(matrix1[0].Replicate(3), matrix2[3], temp);

	temp = VectorSSE::Multiply(matrix1[1].Replicate(0), matrix2[0]);
	temp = VectorSSE::MultiplyAdd(matrix1[1].Replicate(1), matrix2[1], temp);
	temp = VectorSSE::MultiplyAdd(matrix1[1].Replicate(2), matrix2[2], temp);
	VectorSSE r1 = VectorSSE::MultiplyAdd(matrix1[1].Replicate(3), matrix2[3], temp);

	temp = VectorSSE::Multiply(matrix1[2].Replicate(0), matrix2[0]);
	temp = VectorSSE::MultiplyAdd(matrix1[2].Replicate(1), matrix2[1], temp);
	temp = VectorSSE::MultiplyAdd(matrix1[2].Replicate(2), matrix2[2], temp);
	VectorSSE r2 = VectorSSE::MultiplyAdd(matrix1[2].Replicate(3), matrix2[3], temp);

	temp = VectorSSE::Multiply(matrix1[3].Replicate(0), matrix2[0]);
	temp = VectorSSE::MultiplyAdd(matrix1[3].Replicate(1), matrix2[1], temp);
	temp = VectorSSE::MultiplyAdd(matrix1[3].Replicate(2), matrix2[2], temp);
	VectorSSE r3 = VectorSSE::MultiplyAdd(matrix1[3].Replicate(3), matrix2[3], temp);

	return Matrix(r0, r1, r2, r3);
}

Matrix& Matrix::operator*=(const Matrix &i_other)
{
	const VectorSSE *matrix1 = reinterpret_cast<const VectorSSE *>(this);
	const VectorSSE *matrix2 = reinterpret_cast<const VectorSSE *>(&i_other);

	VectorSSE temp;
	temp = VectorSSE::Multiply(matrix1[0].Replicate(0), matrix2[0]);
	temp = VectorSSE::MultiplyAdd(matrix1[0].Replicate(1), matrix2[1], temp);
	temp = VectorSSE::MultiplyAdd(matrix1[0].Replicate(2), matrix2[2], temp);
	VectorSSE r0 = VectorSSE::MultiplyAdd(matrix1[0].Replicate(3), matrix2[3], temp);
	r0.GetFloatArray(M[0]);

	temp = VectorSSE::Multiply(matrix1[1].Replicate(0), matrix2[0]);
	temp = VectorSSE::MultiplyAdd(matrix1[1].Replicate(1), matrix2[1], temp);
	temp = VectorSSE::MultiplyAdd(matrix1[1].Replicate(2), matrix2[2], temp);
	VectorSSE r1 = VectorSSE::MultiplyAdd(matrix1[1].Replicate(3), matrix2[3], temp);
	r1.GetFloatArray(M[1]);

	temp = VectorSSE::Multiply(matrix1[2].Replicate(0), matrix2[0]);
	temp = VectorSSE::MultiplyAdd(matrix1[2].Replicate(1), matrix2[1], temp);
	temp = VectorSSE::MultiplyAdd(matrix1[2].Replicate(2), matrix2[2], temp);
	VectorSSE r2 = VectorSSE::MultiplyAdd(matrix1[2].Replicate(3), matrix2[3], temp);
	r2.GetFloatArray(M[2]);

	temp = VectorSSE::Multiply(matrix1[3].Replicate(0), matrix2[0]);
	temp = VectorSSE::MultiplyAdd(matrix1[3].Replicate(1), matrix2[1], temp);
	temp = VectorSSE::MultiplyAdd(matrix1[3].Replicate(2), matrix2[2], temp);
	VectorSSE r3 = VectorSSE::MultiplyAdd(matrix1[3].Replicate(3), matrix2[3], temp);
	r3.GetFloatArray(M[3]);
	return *this;
}

// normally used, vector is on the left
Vector4 Matrix::MultiplyLeft(const Vector4 &i_vector) const
{
	const VectorSSE vec(i_vector);
	const VectorSSE *matrix = reinterpret_cast<const VectorSSE *>(this);

	VectorSSE temp;
	temp = VectorSSE::Multiply(vec.Replicate(0), matrix[0]);
	temp = VectorSSE::MultiplyAdd(vec.Replicate(1), matrix[1], temp);
	temp = VectorSSE::MultiplyAdd(vec.Replicate(2), matrix[2], temp);
	return Vector4(VectorSSE::MultiplyAdd(vec.Replicate(3), matrix[3], temp));
}

// slower, vector is on the right
Vector4 Matrix::MultiplyRight(const Vector4 &i_vector) const
{
	Matrix transpose = GetTranspose();

	const VectorSSE vec(i_vector);
	const VectorSSE *matrix = reinterpret_cast<const VectorSSE *>(&transpose);

	VectorSSE temp;
	temp = VectorSSE::Multiply(vec.Replicate(0), matrix[0]);
	temp = VectorSSE::MultiplyAdd(vec.Replicate(1), matrix[1], temp);
	temp = VectorSSE::MultiplyAdd(vec.Replicate(2), matrix[2], temp);
	return Vector4(VectorSSE::MultiplyAdd(vec.Replicate(3), matrix[3], temp));
}

// normally used, vectorSSE is on the left
VectorSSE Matrix::MultiplyLeft(const VectorSSE &i_vector) const
{
	const VectorSSE *matrix = reinterpret_cast<const VectorSSE *>(this);

	VectorSSE temp;
	temp = VectorSSE::Multiply(i_vector.Replicate(0), matrix[0]);
	temp = VectorSSE::MultiplyAdd(i_vector.Replicate(1), matrix[1], temp);
	temp = VectorSSE::MultiplyAdd(i_vector.Replicate(2), matrix[2], temp);
	return VectorSSE::MultiplyAdd(i_vector.Replicate(3), matrix[3], temp);
}

// slower, vectorSSE is on the right
VectorSSE Matrix::MultiplyRight(const VectorSSE &i_vector) const
{
	Matrix transpose = GetTranspose();
	const VectorSSE *matrix = reinterpret_cast<const VectorSSE *>(&transpose);

	VectorSSE temp;
	temp = VectorSSE::Multiply(i_vector.Replicate(0), matrix[0]);
	temp = VectorSSE::MultiplyAdd(i_vector.Replicate(1), matrix[1], temp);
	temp = VectorSSE::MultiplyAdd(i_vector.Replicate(2), matrix[2], temp);
	return VectorSSE::MultiplyAdd(i_vector.Replicate(3), matrix[3], temp);
}
