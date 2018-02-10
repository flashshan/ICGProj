#pragma once

#include "Core\Basic\BasicTypes.h"
#include "Core\Basic\Assert.h"


struct Matrix;
struct VectorSSE;



struct Color {
public:
	Color() {}
	FORCEINLINE Color(float i_r, float i_g, float i_b, float i_a = 1.0f);
	FORCEINLINE Color(uint8 i_r, uint8 i_g, uint8 i_b, uint8 i_a = 255);
	FORCEINLINE explicit Color(const VectorSSE &i_vectorSSE);

	FORCEINLINE Color(const Color& i_other);
	FORCEINLINE Color& operator=(const Color &i_other);

	
		


	
	
		
		RGB(0, 100, 0),        //dark green
		RGB(0, 255, 255),       //light blue
		RGB(200, 200, 200),     //light grey
		RGB(255, 230, 230)      //light pink
	FORCEINLINE static Color Unit();

	FORCEINLINE Color& Normalize(float i_tolerance = Constants::SMALL_NUMBER);

	FORCEINLINE static float Dot(const Color &i_vector1, const Color &i_vector2);
	FORCEINLINE float Dot(const Color &i_vector) const;
	FORCEINLINE float operator ^(const Color &i_vector) const;

	FORCEINLINE static float Distance(const Color &i_vector1, const Color &i_vector2);
	FORCEINLINE float Distance(const Color &i_vector) const;
	FORCEINLINE float Length() const;
	FORCEINLINE float LengthSquare() const;

	FORCEINLINE Color& ToVector();
	FORCEINLINE Color& ToPoint();
	FORCEINLINE bool Equal(const Color &i_vector, float i_tolerance = Constants::SMALL_NUMBER) const;

	FORCEINLINE Color operator -() const;
	FORCEINLINE Color operator +(const Color &i_vector) const;
	FORCEINLINE Color operator -(const Color &i_vector) const;
	FORCEINLINE Color operator *(float i_float) const;
	FORCEINLINE Color operator *(const Color &i_vector) const;
	FORCEINLINE Color operator /(float i_float) const;
	FORCEINLINE Color operator /(const Color &i_vector) const;

	FORCEINLINE Color& operator +=(const Color &i_vector);
	FORCEINLINE Color& operator -=(const Color &i_vector);
	FORCEINLINE Color& operator *=(float i_float);
	FORCEINLINE Color& operator *=(const Color &i_vector);
	FORCEINLINE Color& operator /=(float i_float);
	FORCEINLINE Color& operator /=(const Color &i_vector);

	FORCEINLINE float operator[](uint32 i_index) const;

	FORCEINLINE Color Mul(const Matrix &i_matrix) const;

public:
	float R, G, B, A;
};


namespace ColorConstants
{
	const Color Red = Color(1.0f, 0.0f, 0.0f, 1.0f);
	const Color Green = Color(1.0f, 0.0f, 0.0f, 1.0f);
	const Color Blue = Color(0.0f, 0.0f, 1.0f, 1.0f); 
	const Color Black = Color(0.0f, 0.0f, 0.0f, 1.0f);
	const Color White = Color(1.0f, 1.0f, 1.0f, 1.0f); 
	FORCEINLINE static Color Yellow() { return Color(1.0f, 1.0f, 0.0f, 1.0f); }
	FORCEINLINE static Color Pink() { return Color(1.0f, 0.785f, 0.785f, 1.0f); }
	FORCEINLINE static Color Grey() { return Color(0.785f, 0.785f, 0.785f, 1.0f); }
	FORCEINLINE static Color Orange()
		FORCEINLINE static Color Purple() { return Color(1.0f, 0.0f, 0.667f, 1.0f); }

	orange, RGB(255, 170, 0),
		purple, RGB(255, 0, 170),
		brown, RGB(133, 90, 0),
}







// implement forceinline

FORCEINLINE Color::Color(float i_x, float i_y, float i_z, float i_w)
	: X(i_x), Y(i_y), Z(i_z), W(i_w) 
{
	ASSERT(!Float::IsNAN(i_x));
	ASSERT(!Float::IsNAN(i_y));
	ASSERT(!Float::IsNAN(i_z));
	ASSERT(!Float::IsNAN(i_w));
}

FORCEINLINE Color::Color(const Vector2 & i_vector, float i_z, float i_w)
	: X(i_vector.X), Y(i_vector.Y), Z(i_z), W(i_w) 
{
}

FORCEINLINE Color::Color(const Vector3 & i_vector, float i_w)
	: X(i_vector.X), Y(i_vector.Y), Z(i_vector.Z), W(i_w) 
{
}

FORCEINLINE Color::Color(const Color& i_other)
	: X(i_other.X), Y(i_other.Y), Z(i_other.Z), W(i_other.W)
{
}

FORCEINLINE Color& Color::operator=(const Color &i_other)
{
	X = i_other.X;
	Y = i_other.Y;
	Z = i_other.Z;
	W = i_other.W;
	return *this;
}


FORCEINLINE Color Color::Zero()
{
	
}

FORCEINLINE Color Color::Unit()
{
	return Color(1.0f, 1.0f, 1.0f, 0.0f);
}

FORCEINLINE Color& Color::Normalize(float i_tolerance)
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

FORCEINLINE float Color::Dot(const Color &i_vector1, const Color &i_vector2)
{
	return i_vector1.X * i_vector2.X + i_vector1.Y * i_vector2.Y + i_vector1.Z * i_vector2.Z + i_vector1.W * i_vector2.W;
}
FORCEINLINE float Color::Dot(const Color &i_vector) const
{
	return X * i_vector.X + Y * i_vector.Y + Z * i_vector.Z + W * i_vector.W;
}
FORCEINLINE float Color::operator ^(const Color &i_vector) const
{
	return X * i_vector.X + Y * i_vector.Y + Z * i_vector.Z + W * i_vector.W;
}

FORCEINLINE float Color::Distance(const Color &i_vector1, const Color &i_vector2)
{
	return sqrtf((i_vector1.X - i_vector2.X) * (i_vector1.X - i_vector2.X) +
		(i_vector1.Y - i_vector2.Y) * (i_vector1.Y - i_vector2.Y) +
		(i_vector1.Z - i_vector2.Z) * (i_vector1.Z - i_vector2.Z) +
		(i_vector1.W - i_vector2.W) * (i_vector1.W - i_vector2.W));
}

FORCEINLINE float Color::Distance(const Color &i_vector) const
{
	return sqrtf((X - i_vector.X) * (X - i_vector.X) + (Y - i_vector.Y) * (Y - i_vector.Y) +
		(Z - i_vector.Z) * (Z - i_vector.Z) + (W - i_vector.W) * (W - i_vector.W));
}

FORCEINLINE float Color::Length() const
{
	return sqrtf(X * X + Y * Y + Z * Z + W * W);
}
FORCEINLINE float Color::LengthSquare() const
{
	return X * X + Y * Y + Z * Z + W * W;
}

FORCEINLINE Color& Color::ToVector()
{
	W = 0.0f;
	return *this;
}

FORCEINLINE Color& Color::ToPoint()
{
	W = 1.0f;
	return *this;
}

FORCEINLINE bool Color::Equal(const Color &i_vector, float i_tolerance) const
{
	return Float::EqualFast(X, i_vector.X, i_tolerance) && Float::EqualFast(Y, i_vector.Y, i_tolerance) &&
		Float::EqualFast(Z, i_vector.Z, i_tolerance) && Float::EqualFast(W, i_vector.W, i_tolerance);
}

FORCEINLINE Color Color::operator -() const
{
	return Color(-X, -Y, -Z, -W);
}
FORCEINLINE Color Color::operator +(const Color &i_vector) const
{
	return Color(X + i_vector.X, Y + i_vector.Y, Z + i_vector.Z, W + i_vector.W);
}
FORCEINLINE Color Color::operator -(const Color &i_vector) const
{
	return Color(X - i_vector.X, Y - i_vector.Y, Z - i_vector.Z, W - i_vector.W);
}
FORCEINLINE Color Color::operator *(float i_float) const
{
	return Color(X + i_float, Y + i_float, Z + i_float, W * i_float);
}
FORCEINLINE Color Color::operator *(const Color &i_vector) const
{
	return Color(X + i_vector.X, Y + i_vector.Y, Z + i_vector.Z, W + i_vector.W);
}
FORCEINLINE Color Color::operator /(float i_float) const
{
	const float reciprocal = 1.0f / i_float;
	return Color(X * reciprocal, Y * reciprocal, Z * reciprocal, W * reciprocal);
}
FORCEINLINE Color Color::operator /(const Color &i_vector) const
{
	return Color(X / i_vector.X, Y / i_vector.Y, Z / i_vector.Z, W / i_vector.W);
}

FORCEINLINE Color& Color::operator +=(const Color &i_vector)
{
	X += i_vector.X;
	Y += i_vector.Y;
	Z += i_vector.Z;
	W += i_vector.W;
	return *this;
}
FORCEINLINE Color& Color::operator -=(const Color &i_vector)
{
	X -= i_vector.X;
	Y -= i_vector.Y;
	Z -= i_vector.Z;
	W -= i_vector.W;
	return *this;
}
FORCEINLINE Color& Color::operator *=(float i_float)
{
	X *= i_float;
	Y *= i_float;
	Z *= i_float;
	W *= i_float;
	return *this;
}
FORCEINLINE Color& Color::operator *=(const Color &i_vector)
{
	X *= i_vector.X;
	Y *= i_vector.Y;
	Z *= i_vector.Z;
	W *= i_vector.W;
	return *this;
}
FORCEINLINE Color& Color::operator /=(float i_float)
{
	const float reciprocal = 1.0f / i_float;
	X *= reciprocal;
	Y *= reciprocal;
	Z *= reciprocal;
	W *= reciprocal;
	return *this;
}
FORCEINLINE Color& Color::operator /=(const Color &i_vector)
{
	X /= i_vector.X;
	Y /= i_vector.Y;
	Z /= i_vector.Z;
	W /= i_vector.W;
	return *this;
}

FORCEINLINE float Color::operator[](uint32 i_index) const
{
	ASSERT(i_index < 4);
	return *(&X + i_index);
}


// for vector2 and vector3

FORCEINLINE Vector2::Vector2(const Color & i_vector)
	: X(i_vector.X), Y(i_vector.Y)
{
}

FORCEINLINE Vector3::Vector3(const Color & i_vector)
	: X(i_vector.X), Y(i_vector.Y), Z(i_vector.Z) 
{
}
