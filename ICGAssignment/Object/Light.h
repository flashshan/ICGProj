#pragma once

#include "Core/Math/Quaternion.h"


class DirectionalLight
{
public:
	DirectionalLight(float i_pitch, float i_yaw)
		: pitch_(i_pitch), yaw_(i_yaw)
	{
	}

	void AddPitchYaw(float i_pitch, float i_yaw)
	{
		pitch_ += i_pitch;
		yaw_ += i_yaw;
	}

	Vector3 GetDirection() const
	{
		Quaternion lightQuat(Rotator(pitch_, yaw_, 0.0f));
		return lightQuat.GetAxisX();
	}

private:
	float pitch_;
	float yaw_;
};

class SpotLight
{
public:
	const float spotLightNearPlane = 0.01f;
	const float spotLightFarPlane = 100.f;
public:
	SpotLight(const Vector3 &i_pos, float i_pitch, float i_yaw, float i_angle)
		: pos_(i_pos), pitch_(i_pitch), yaw_(i_yaw), angle_(i_angle)
	{
	}

	void AddPitchYaw(float i_pitch, float i_yaw)
	{
		pitch_ += i_pitch;
		yaw_ += i_yaw;
	}

	void MoveLight(float i_x, float i_y, float i_z)
	{
		pos_ += Vector3(i_x, i_y, i_z);
	}

	Vector3 GetDirection() const 
	{
		Quaternion lightQuat(Rotator(pitch_, yaw_, 0.0f));
		return lightQuat.GetAxisX();
	}

	Vector3 GetPosition() const
	{
		return pos_;
	}

	Matrix GetSpotLightTransformMatrix() const
	{
		const float scale = 1.0f / Math::Tan(Math::DegreesToRadians(angle_) / 2);
		const float zDistanceScale = 1.0f / (spotLightNearPlane - spotLightFarPlane);

		Matrix perspectiveProjection_
		(
			scale, 0.0f, 0.0f, 0.0f,
			0.0f, scale, 0.0f, 0.0f,
			0.0f, 0.0f, -(spotLightNearPlane + spotLightFarPlane) * zDistanceScale, (2.0f * spotLightNearPlane * spotLightFarPlane) * zDistanceScale,
			0.0f, 0.0f, 1.0f, 0.0f
		);

		Rotator rotator(pitch_, yaw_, 0.0f);
		return perspectiveProjection_ * Matrix(rotator).GetTranspose() * Matrix::CreateTranslate(-pos_).GetTranspose();
	}

private:
	Vector3 pos_;

	float pitch_;
	float yaw_;
	float angle_;
};
