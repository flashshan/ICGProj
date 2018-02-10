#pragma once

#include "Vector3.h"

struct Matrix;
struct VectorSSE;

struct Vector4 {
public:
	Vector4() {}
	FORCEINLINE Vector4(float i_x, float i_y, float i_z, float i_w = 1.0f);
	FORCEINLINE explicit Vector4(const Vector2 & i_vector, float i_z = 0.0f, float i_w = 1.0f);
	FORCEINLINE explicit Vector4(const Vector3 & i_vector, float i_w = 1.0f);
	FORCEINLINE explicit Vector4(const VectorSSE &i_vectorSSE);

	FORCEINLINE Vector4(const Vector4& i_other);
	FORCEINLINE Vector4& operator=(const Vector4 &i_other);

	FORCEINLINE static Vector4 Zero();
	FORCEINLINE static Vector4 Unit();

	FORCEINLINE Vector4& Normalize(float i_tolerance = Constants::SMALL_NUMBER);

	FORCEINLINE static float Dot(const Vector4 &i_vector1, const Vector4 &i_vector2);
	FORCEINLINE float Dot(const Vector4 &i_vector) const;
	FORCEINLINE float operator ^(const Vector4 &i_vector) const;

	FORCEINLINE static float Distance(const Vector4 &i_vector1, const Vector4 &i_vector2);
	FORCEINLINE float Distance(const Vector4 &i_vector) const;
	FORCEINLINE float Length() const;
	FORCEINLINE float LengthSquare() const;

	FORCEINLINE Vector4& ToVector();
	FORCEINLINE Vector4& ToPoint();
	FORCEINLINE bool Equal(const Vector4 &i_vector, float i_tolerance = Constants::SMALL_NUMBER) const;

	FORCEINLINE Vector4 operator -() const;
	FORCEINLINE Vector4 operator +(const Vector4 &i_vector) const;
	FORCEINLINE Vector4 operator -(const Vector4 &i_vector) const;
	FORCEINLINE Vector4 operator *(float i_float) const;
	FORCEINLINE Vector4 operator *(const Vector4 &i_vector) const;
	FORCEINLINE Vector4 operator /(float i_float) const;
	FORCEINLINE Vector4 operator /(const Vector4 &i_vector) const;

	FORCEINLINE Vector4& operator +=(const Vector4 &i_vector);
	FORCEINLINE Vector4& operator -=(const Vector4 &i_vector);
	FORCEINLINE Vector4& operator *=(float i_float);
	FORCEINLINE Vector4& operator *=(const Vector4 &i_vector);
	FORCEINLINE Vector4& operator /=(float i_float);
	FORCEINLINE Vector4& operator /=(const Vector4 &i_vector);

	FORCEINLINE float operator[](uint32 i_index) const;

	FORCEINLINE Vector4 Mul(const Matrix &i_matrix) const;

public:
	float X, Y, Z, W;
};








// implement forceinline

FORCEINLINE Vector4::Vector4(float i_x, float i_y, float i_z, float i_w)
	: X(i_x), Y(i_y), Z(i_z), W(i_w) 
{
	ASSERT(!Float::IsNAN(i_x));
	ASSERT(!Float::IsNAN(i_y));
	ASSERT(!Float::IsNAN(i_z));
	ASSERT(!Float::IsNAN(i_w));
}

FORCEINLINE Vector4::Vector4(const Vector2 & i_vector, float i_z, float i_w)
	: X(i_vector.X), Y(i_vector.Y), Z(i_z), W(i_w) 
{
}

FORCEINLINE Vector4::Vector4(const Vector3 & i_vector, float i_w)
	: X(i_vector.X), Y(i_vector.Y), Z(i_vector.Z), W(i_w) 
{
}

FORCEINLINE Vector4::Vector4(const Vector4& i_other)
	: X(i_other.X), Y(i_other.Y), Z(i_other.Z), W(i_other.W)
{
}

FORCEINLINE Vector4& Vector4::operator=(const Vector4 &i_other)
{
	X = i_other.X;
	Y = i_other.Y;
	Z = i_other.Z;
	W = i_other.W;
	return *this;
}


FORCEINLINE Vector4 Vector4::Zero()
{
	return Vector4(0.0f, 0.0f, 0.0f, 0.0f);
}

FORCEINLINE Vector4 Vector4::Unit()
{
	return Vector4(1.0f, 1.0f, 1.0f, 0.0f);
}

FORCEINLINE Vector4& Vector4::Normalize(float i_tolerance)
{
	const float magSq = Length();
	if (magSq > i_tolerance)
	{
		X /= magSq;
		Y /= magSq;
		Z /= magSq;
		W /= magSq;
	}
	else
	{
		X = 0.0f;
		Y = 0.0f;
		Z = 0.0f;
		W = 0.0f;
	}
	return *this;
}

FORCEINLINE float Vector4::Dot(const Vector4 &i_vector1, const Vector4 &i_vector2)
{
	return i_vector1.X * i_vector2.X + i_vector1.Y * i_vector2.Y + i_vector1.Z * i_vector2.Z + i_vector1.W * i_vector2.W;
}
FORCEINLINE float Vector4::Dot(const Vector4 &i_vector) const
{
	return X * i_vector.X + Y * i_vector.Y + Z * i_vector.Z + W * i_vector.W;
}
FORCEINLINE float Vector4::operator ^(const Vector4 &i_vector) const
{
	return X * i_vector.X + Y * i_vector.Y + Z * i_vector.Z + W * i_vector.W;
}

FORCEINLINE float Vector4::Distance(const Vector4 &i_vector1, const Vector4 &i_vector2)
{
	return sqrtf((i_vector1.X - i_vector2.X) * (i_vector1.X - i_vector2.X) +
		(i_vector1.Y - i_vector2.Y) * (i_vector1.Y - i_vector2.Y) +
		(i_vector1.Z - i_vector2.Z) * (i_vector1.Z - i_vector2.Z) +
		(i_vector1.W - i_vector2.W) * (i_vector1.W - i_vector2.W));
}

FORCEINLINE float Vector4::Distance(const Vector4 &i_vector) const
{
	return sqrtf((X - i_vector.X) * (X - i_vector.X) + (Y - i_vector.Y) * (Y - i_vector.Y) +
		(Z - i_vector.Z) * (Z - i_vector.Z) + (W - i_vector.W) * (W - i_vector.W));
}

FORCEINLINE float Vector4::Length() const
{
	return sqrtf(X * X + Y * Y + Z * Z + W * W);
}
FORCEINLINE float Vector4::LengthSquare() const
{
	return X * X + Y * Y + Z * Z + W * W;
}

FORCEINLINE Vector4& Vector4::ToVector()
{
	W = 0.0f;
	return *this;
}

FORCEINLINE Vector4& Vector4::ToPoint()
{
	W = 1.0f;
	return *this;
}

FORCEINLINE bool Vector4::Equal(const Vector4 &i_vector, float i_tolerance) const
{
	return Float::EqualFast(X, i_vector.X, i_tolerance) && Float::EqualFast(Y, i_vector.Y, i_tolerance) &&
		Float::EqualFast(Z, i_vector.Z, i_tolerance) && Float::EqualFast(W, i_vector.W, i_tolerance);
}

FORCEINLINE Vector4 Vector4::operator -() const
{
	return Vector4(-X, -Y, -Z, -W);
}
FORCEINLINE Vector4 Vector4::operator +(const Vector4 &i_vector) const
{
	return Vector4(X + i_vector.X, Y + i_vector.Y, Z + i_vector.Z, W + i_vector.W);
}
FORCEINLINE Vector4 Vector4::operator -(const Vector4 &i_vector) const
{
	return Vector4(X - i_vector.X, Y - i_vector.Y, Z - i_vector.Z, W - i_vector.W);
}
FORCEINLINE Vector4 Vector4::operator *(float i_float) const
{
	return Vector4(X + i_float, Y + i_float, Z + i_float, W * i_float);
}
FORCEINLINE Vector4 Vector4::operator *(const Vector4 &i_vector) const
{
	return Vector4(X + i_vector.X, Y + i_vector.Y, Z + i_vector.Z, W + i_vector.W);
}
FORCEINLINE Vector4 Vector4::operator /(float i_float) const
{
	const float reciprocal = 1.0f / i_float;
	return Vector4(X * reciprocal, Y * reciprocal, Z * reciprocal, W * reciprocal);
}
FORCEINLINE Vector4 Vector4::operator /(const Vector4 &i_vector) const
{
	return Vector4(X / i_vector.X, Y / i_vector.Y, Z / i_vector.Z, W / i_vector.W);
}

FORCEINLINE Vector4& Vector4::operator +=(const Vector4 &i_vector)
{
	X += i_vector.X;
	Y += i_vector.Y;
	Z += i_vector.Z;
	W += i_vector.W;
	return *this;
}
FORCEINLINE Vector4& Vector4::operator -=(const Vector4 &i_vector)
{
	X -= i_vector.X;
	Y -= i_vector.Y;
	Z -= i_vector.Z;
	W -= i_vector.W;
	return *this;
}
FORCEINLINE Vector4& Vector4::operator *=(float i_float)
{
	X *= i_float;
	Y *= i_float;
	Z *= i_float;
	W *= i_float;
	return *this;
}
FORCEINLINE Vector4& Vector4::operator *=(const Vector4 &i_vector)
{
	X *= i_vector.X;
	Y *= i_vector.Y;
	Z *= i_vector.Z;
	W *= i_vector.W;
	return *this;
}
FORCEINLINE Vector4& Vector4::operator /=(float i_float)
{
	const float reciprocal = 1.0f / i_float;
	X *= reciprocal;
	Y *= reciprocal;
	Z *= reciprocal;
	W *= reciprocal;
	return *this;
}
FORCEINLINE Vector4& Vector4::operator /=(const Vector4 &i_vector)
{
	X /= i_vector.X;
	Y /= i_vector.Y;
	Z /= i_vector.Z;
	W /= i_vector.W;
	return *this;
}

FORCEINLINE float Vector4::operator[](uint32 i_index) const
{
	ASSERT(i_index < 4);
	return *(&X + i_index);
}


// for vector2 and vector3

FORCEINLINE Vector2::Vector2(const Vector4 & i_vector)
	: X(i_vector.X), Y(i_vector.Y)
{
}

FORCEINLINE Vector3::Vector3(const Vector4 & i_vector)
	: X(i_vector.X), Y(i_vector.Y), Z(i_vector.Z) 
{
}
