#pragma once

#include "ILevel.h"

#include "Object/Camera.h"
#include "Object/Light.h"
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
		: mainCamera_(i_camera), mainLight_(0.0f, 0.0f)
	{
	}

	virtual void InitLevel() override;
	virtual void DisplayLevel() override;
	virtual void CloseLevel() override;

	virtual void UpdateMainLight(float i_pitch, float i_yaw) override
	{
		mainLight_.AddPitchYaw(i_pitch, i_yaw);
	}

	virtual void HandleNormalKey(char i_char) override
	{
	}

private:
	Camera* mainCamera_;
	DirectionalLight mainLight_;
	
	cy::TriMesh teapot_, cube_, sphere_;
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

