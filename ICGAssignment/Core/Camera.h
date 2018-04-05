#pragma once

#include "Core/Math/Matrix.h"


struct ProjectionParameters
{
public:
	float HorizontalFOVDegree;
	float AspectRatio;
	float NearPlane;
	float FarPlane;

	ProjectionParameters(float horizontalFOVDegree, float aspectRatio, float nearPlane, float farPlane)
		: HorizontalFOVDegree(horizontalFOVDegree), AspectRatio(aspectRatio), NearPlane(nearPlane), FarPlane(farPlane) {}
};

class Camera
{
public:
	Camera(Vector3 pos, float yaw, float pitch, ProjectionParameters projectionParameters) 
		: originPosition_(pos), position_(pos), upDir_(0.0f, 0.0f, 1.0f), originYaw_(yaw), yaw_(yaw), originPitch_(pitch), pitch_(pitch)
	{
		const float xScale = 1.0f / Math::Tan(Math::DegreesToRadians(projectionParameters.HorizontalFOVDegree) / 2);
		const float yScale = xScale / projectionParameters.AspectRatio;
		const float zDistanceScale = 1.0f / (projectionParameters.NearPlane - projectionParameters.FarPlane);

		perspectiveProjection_ = Matrix
		(
			xScale, 0.0f, 0.0f, 0.0f,
			0.0f, yScale, 0.0f, 0.0f,
			0.0f, 0.0f, -(projectionParameters.NearPlane + projectionParameters.FarPlane) * zDistanceScale, (2.0f * projectionParameters.NearPlane * projectionParameters.FarPlane) * zDistanceScale,
			0.0f, 0.0f, 1.0f, 0.0f
		);

		float right = 5.0f, left = -right, top = right / projectionParameters.AspectRatio, bottom = -top;
		orthogonalProjection_ = Matrix
		(
			2.0f / (right - left), 0.0f, 0.0f, (left + right) / (left - right),
			0.0f, 2.0f / (top - bottom), 0.0f, (bottom + top) / (bottom - top),
			0.0f, 0.0f, 1.0, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f
		);

		viewToProjection_ = perspectiveProjection_;
		perspectiveView_ = true;
	}

	Matrix GetWorldToProjectionMatrix() const;
	Matrix GetRefWorldToProjectionMatrix() const;

	void Rotate(float yaw, float pitch)
	{
		yaw_ += yaw;
		pitch_ += pitch;
	}

	void Move(float distance)
	{
		distance_ += distance;
		position_ = originPosition_ * distance_;
	}

	void Reset()
	{
		distance_ = 1.0f;
		yaw_ = originYaw_;
		pitch_ = originPitch_;
	}

	void SwitchView()
	{
		if (perspectiveView_)
		{
			viewToProjection_ = orthogonalProjection_;
			perspectiveView_ = false;
		}
		else
		{
			viewToProjection_ = perspectiveProjection_;
			perspectiveView_ = true;
		}
	}

	Vector3 GetPosition() const { return position_; }

protected:
	Matrix viewToProjection_;
	Matrix perspectiveProjection_;
	Matrix orthogonalProjection_;

	Vector3 originPosition_;
	Vector3 position_;
	Vector3 upDir_;

	float originYaw_;
	float yaw_;

	float originPitch_;
	float pitch_;

	float distance_ = 1.0f;
	bool perspectiveView_;
};
