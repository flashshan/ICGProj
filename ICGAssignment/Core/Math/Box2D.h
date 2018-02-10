#pragma once

#include "Matrix.h"

struct Box2D {
public:
	FORCEINLINE Box2D();
	FORCEINLINE Box2D(const Vector2 &i_center, const Vector2 &i_extend);
	
	FORCEINLINE void SetCenter(const Vector2 &i_vector) { center_ = i_vector; }
	FORCEINLINE void SetExtend(const Vector2 &i_vector) { extend_ = i_vector; }
	FORCEINLINE Vector2 GetCenter() const { return center_; }
	FORCEINLINE Vector2 GetExtend() const { return extend_; }

	inline Box2D& Transform(const Matrix &i_matrix);
	inline Box2D GetTransform(const Matrix &i_matrix) const;

private:
	Vector2 center_, extend_;
};




// implement forceinline

FORCEINLINE Box2D::Box2D()
	: center_(0.0f, 0.0f), extend_(0.0f, 0.0f)
{
}

FORCEINLINE Box2D::Box2D(const Vector2 &i_center, const Vector2 &i_extend)
	: center_(i_center), extend_(i_extend)
{
}

inline Box2D& Box2D::Transform(const Matrix &i_matrix)
{
	center_ = Vector2(Vector4(center_).ToPoint().Mul(i_matrix));
	extend_ = Vector2(Vector4(extend_).ToPoint().Mul(i_matrix));
	return *this;
}

inline Box2D Box2D::GetTransform(const Matrix &i_matrix) const
{
	return Box2D(Vector2(Vector4(center_).ToPoint().Mul(i_matrix)), Vector2(Vector4(extend_).ToPoint().Mul(i_matrix)));
}
