#pragma once

#include "Matrix.h"

class Triangle {
public:
	FORCEINLINE Triangle(const Vector3 &i_a, const Vector3 &i_b, const Vector3 &i_c);

	// get
	FORCEINLINE Vector3 A() const { return V_[0]; }
	FORCEINLINE Vector3 B() const { return V_[1]; }
	FORCEINLINE Vector3 C() const { return V_[2]; }
	FORCEINLINE Vector3 OriginA() const { return originV_[0]; }
	FORCEINLINE Vector3 OriginB() const { return originV_[1]; }
	FORCEINLINE Vector3 OriginC() const { return originV_[2]; }

	FORCEINLINE static Triangle Identity();

	FORCEINLINE Triangle& Transform(const Matrix &i_matrix);
	
	FORCEINLINE Triangle& ChangeOrigin(const Vector3 &i_a, const Vector3 &i_b, const Vector3 &i_c);

public:
	Vector3 V_[3];
	Vector3 originV_[3];
};






// implement forceinline

FORCEINLINE Triangle::Triangle(const Vector3 &i_a, const Vector3 &i_b, const Vector3 &i_c)
	: V_{i_a, i_b, i_c },  originV_{i_a, i_b, i_c } 
{
}

FORCEINLINE Triangle Triangle::Identity()
{
	return Triangle(Vector3(1.0f, 0.0f, 0.0f), Vector3(0.0f, 1.0f, 0.0f), Vector3(0.0f, 0.0f, 1.0f));
}

FORCEINLINE Triangle& Triangle::Transform(const Matrix &i_matrix)
{
	V_[0] = Vector3(Vector4(originV_[0], 1.f).Mul(i_matrix));
	V_[1] = Vector3(Vector4(originV_[1], 1.f).Mul(i_matrix));
	V_[2] = Vector3(Vector4(originV_[2], 1.f).Mul(i_matrix));
}

FORCEINLINE Triangle& Triangle::ChangeOrigin(const Vector3 &i_a, const Vector3 &i_b, const Vector3 &i_c)
{
	originV_[0] = i_a;
	originV_[1] = i_b;
	originV_[2] = i_c;
}

