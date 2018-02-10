#pragma once

#include "Core\Basic\BasicTypes.h"
#include "Core\Basic\Assert.h"

#include "Core\Basic\BasicFunctions.h"

struct Vector3;
struct Vector4;
struct VectorSSE;

struct Vector2 {
public:
	FORCEINLINE Vector2() {}
	FORCEINLINE Vector2(float i_x, float i_y);
	FORCEINLINE explicit Vector2(const Vector3 & i_vector);
	FORCEINLINE explicit Vector2(const Vector4 & i_vector);
	FORCEINLINE explicit Vector2(const VectorSSE &i_vectorSSE);
	
	FORCEINLINE Vector2(const Vector2 &i_other);
	FORCEINLINE Vector2& operator=(const Vector2 &i_other);

	static FORCEINLINE Vector2 Zero();
	static FORCEINLINE Vector2 Unit();
	static FORCEINLINE Vector2 RandomNormal();

	static FORCEINLINE float Dot(const Vector2 &i_vector1, const Vector2 &i_vector2);
	FORCEINLINE float Dot(const Vector2 &i_vector) const;
	FORCEINLINE float operator |(const Vector2 &i_vector) const;

	static FORCEINLINE float Cross(const Vector2 &i_vector1, const Vector2 &i_vector2);
	FORCEINLINE float Cross(const Vector2 &i_vector) const;
	FORCEINLINE float operator ^(const Vector2 &i_vector) const;

	static FORCEINLINE float Distance(const Vector2 &i_vector1, const Vector2 &i_vector2);
	FORCEINLINE float Distance(const Vector2 &i_vector) const;

	FORCEINLINE Vector2& Normalize(float tolerance = Constants::SMALL_NUMBER);
	FORCEINLINE float Length() const;
	FORCEINLINE float LengthSquare() const;

	FORCEINLINE bool Equal(const Vector2 &i_vector, float i_tolerance = Constants::SMALL_NUMBER);

	FORCEINLINE Vector2 operator -() const;
	FORCEINLINE Vector2 operator +(const Vector2 &i_vector) const;
	FORCEINLINE Vector2 operator -(const Vector2 &i_vector) const;
	FORCEINLINE Vector2 operator *(float i_float) const;
	FORCEINLINE Vector2 operator *(const Vector2 &i_vector) const;
	FORCEINLINE Vector2 operator /(float i_float) const;
	FORCEINLINE Vector2 operator /(const Vector2 &i_vector) const;

	FORCEINLINE Vector2& operator +=(const Vector2 &i_vector);
	FORCEINLINE Vector2& operator -=(const Vector2 &i_vector);
	FORCEINLINE Vector2& operator *=(float i_float);
	FORCEINLINE Vector2& operator *=(const Vector2 &i_vector);
	FORCEINLINE Vector2& operator /=(float i_float);
	FORCEINLINE Vector2& operator /=(const Vector2 &i_vector);

	FORCEINLINE float operator [](uint32 i_index) const;
public:
	float X, Y;
};





// implement forceinline


FORCEINLINE Vector2::Vector2(float i_x, float i_y)
	: X(i_x), Y(i_y)
{
	ASSERT(!Float::IsNAN(i_x));
	ASSERT(!Float::IsNAN(i_y));
}

FORCEINLINE Vector2::Vector2(const Vector2 &i_other)
	: X(i_other.X), Y(i_other.Y)
{
}

FORCEINLINE Vector2& Vector2::operator=(const Vector2 &i_other)
{
	X = i_other.X;
	Y = i_other.Y;
	return *this;
}

FORCEINLINE Vector2 Vector2::Zero()
{
	return Vector2(0.0f, 0.0f);
}

FORCEINLINE Vector2 Vector2::Unit()
{
	return Vector2(1.0f, 1.0f);
}

FORCEINLINE Vector2 Vector2::RandomNormal()
{
	return Vector2(Math::RandOne() * 2.0f - 1.0f, Math::RandOne() * 2.0f - 1.0f);
}

FORCEINLINE float Vector2::Dot(const Vector2 &i_vector1, const Vector2 &i_vector2)
{
	return i_vector1.X * i_vector2.X + i_vector1.Y * i_vector2.Y;
}

FORCEINLINE float Vector2::Dot(const Vector2 &i_vector) const
{
	return X * i_vector.X + Y * i_vector.Y;
}

FORCEINLINE float Vector2::operator |(const Vector2 &i_vector) const
{
	return X * i_vector.X + Y * i_vector.Y;
}

FORCEINLINE float Vector2::Cross(const Vector2 &i_vector1, const Vector2 &i_vector2)
{
	return i_vector1.X * i_vector2.X - i_vector1.Y * i_vector2.Y;
}
FORCEINLINE float Vector2::Cross(const Vector2 &i_vector) const
{
	return X * i_vector.X - Y * i_vector.Y;
}

FORCEINLINE float Vector2::operator ^(const Vector2 &i_vector) const
{
	return X * i_vector.X - Y * i_vector.Y;
}

FORCEINLINE float Vector2::Distance(const Vector2 &i_vector1, const Vector2 &i_vector2)
{
	return sqrtf((i_vector1.X - i_vector2.X) * (i_vector1.X - i_vector2.X) + (i_vector1.Y - i_vector2.Y) * (i_vector1.Y - i_vector2.Y));
}
FORCEINLINE float Vector2::Distance(const Vector2 &i_vector) const
{
	return sqrtf((X - i_vector.X) * (X - i_vector.X) + (X - i_vector.X) * (X - i_vector.X));
}

FORCEINLINE Vector2& Vector2::Normalize(float tolerance)
{
	float magSq = 1.0f / Length();
	if (magSq > tolerance)
	{
		X /= magSq;
		Y /= magSq;
	}
	else
	{
		X = 0.0f;
		Y = 0.0f;
	}
	return *this;
}

FORCEINLINE float Vector2::Length() const
{
	return static_cast<float>(sqrt(X * X + Y * Y));
}

FORCEINLINE float Vector2::LengthSquare() const
{
	return X * X + Y * Y;
}

FORCEINLINE bool Vector2::Equal(const Vector2 &i_vector, float i_tolerance)
{
	return Float::EqualFast(X, i_vector.X, i_tolerance) && Float::EqualFast(Y, i_vector.Y, i_tolerance);
}

FORCEINLINE Vector2 Vector2::operator -() const
{
	return Vector2(-X, -Y);
}

FORCEINLINE Vector2 Vector2::operator +(const Vector2 &i_vector) const
{
	return Vector2(X + i_vector.X, Y + i_vector.Y);
}

FORCEINLINE Vector2 Vector2::operator -(const Vector2 &i_vector) const
{
	return Vector2(X - i_vector.X, Y - i_vector.Y);
}

FORCEINLINE Vector2 Vector2::operator *(float i_float) const
{
	return Vector2(X * i_float, Y * i_float);
}

FORCEINLINE Vector2 Vector2::operator *(const Vector2 &i_vector) const
{
	return Vector2(X * i_vector.X, Y * i_vector.Y);
}

FORCEINLINE Vector2 Vector2::operator /(float i_float) const
{
	return Vector2(X / i_float, Y / i_float);
}

FORCEINLINE Vector2 Vector2::operator /(const Vector2 &i_vector) const
{
	return Vector2(X / i_vector.X, Y / i_vector.Y);
}

FORCEINLINE Vector2& Vector2::operator +=(const Vector2 &i_vector)
{
	X += i_vector.X;
	Y += i_vector.Y;
	return *this;
}

FORCEINLINE Vector2& Vector2::operator -=(const Vector2 &i_vector)
{
	X -= i_vector.X;
	Y -= i_vector.Y;
	return *this;
}

FORCEINLINE Vector2& Vector2::operator *=(float i_float)
{
	X *= i_float;
	Y *= i_float;
	return *this;
}

FORCEINLINE Vector2& Vector2::operator *=(const Vector2 &i_vector)
{
	X *= i_vector.X;
	Y *= i_vector.Y;
	return *this;
}

FORCEINLINE Vector2& Vector2::operator /=(float i_float)
{
	X /= i_float;
	Y /= i_float;
	return *this;
}

FORCEINLINE Vector2& Vector2::operator /=(const Vector2 &i_vector)
{
	X /= i_vector.X;
	Y /= i_vector.Y;
	return *this;
}

FORCEINLINE float Vector2::operator [](uint32 i_index) const
{
	ASSERT(i_index <= 1);
	return i_index == 0 ? X : Y;
}

