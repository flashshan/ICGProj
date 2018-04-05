#pragma once

#include "ILevel.h"

#include "Core/Camera.h"
#include "Graphics/Texture.h"
#include "Graphics/FrameBuffer.h"
#include "Graphics/GraphicsHelper.h"

#include <cyTriMesh.h>

class Camera;

// a static class

class EnvironmentLevel : public ILevel
{
public:
	explicit EnvironmentLevel(Camera *i_camera)
		: mainCamera_(i_camera) {}

	virtual void InitLevel() override;
	virtual void DisplayLevel() override;
	virtual void CloseLevel() override;

	void UpdateMainLight(float i_pitch, float i_yaw)
	{
		lightPitch_ += i_pitch;
		lightYaw_ += i_yaw;
	}
	
private:
	Camera* mainCamera_;

	cy::TriMesh teapot_, cube_, sphere_;
	float lightYaw_ = 0.0f, lightPitch_ = 0.0f;
	GLuint skyBoxProgram_, reflectionPrgram_, teapotProgram_, planeProgram_;

	// skyBox shader locations
	GLuint skyBoxFTMLocation_;

	// reflection shader locations
	GLuint reflectionFTMLocation_;
	GLuint refLightDirectionLocation_;
	GLuint refEyePositionLocation_;

	// teapot shader locations
	GLuint teapotFTMLocation_;
	GLuint lightDirectionLocation_;
	GLuint eyePositionLocation_;

	// quad shader locations
	GLuint planeFTMLocation_;
	GLuint renderTextureLocation_;

	GLuint VAOCube_, VAOSphere_, VAOTeapot_, VAOPlane_;

	RenderedTexture2D *renderedTexture_ = nullptr;

	CubeMapTexture *cubeMapTexture_ = nullptr;

	FrameBuffer *frameBuffer_ = nullptr;
};

