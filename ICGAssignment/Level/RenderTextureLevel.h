#pragma once

#include "ILevel.h"

#include <cyTriMesh.h>

#include "Object/Camera.h"
#include "Object/Light.h"
#include "Graphics/Texture.h"
#include "Graphics/FrameBuffer.h"
#include "Graphics/GraphicsHelper.h"


class RenderTextureLevel : public ILevel
{
public:
	RenderTextureLevel(Camera *i_camera)
		: mainCamera_(i_camera), mainLight_(-50.0f, -40.0f)
	{
	}

	virtual void InitLevel();
	virtual void DisplayLevel();
	virtual void CloseLevel();

	virtual void UpdateMainLight(float i_pitch, float i_yaw) override
	{
		mainLight_.AddPitchYaw(i_pitch, i_yaw);
	}

	virtual void HandleNormalKey(char i_char) override
	{
	}

private:
	Camera *mainCamera_;
	DirectionalLight mainLight_;

	cy::TriMesh teapot_;

	GLuint teapotProgram_, planeProgram_;
	GLuint VAOTeapot_, VAOPlane_;

	GLuint normalFTMLocation_;
	GLuint lightDirectionLocation_;
	GLuint eyePositionLocation_;
	GLuint diffuseLocation_;
	GLuint specularLocation_;

	GLuint planeFTMLocation_;
	GLuint renderedTextureLocation_;

	
	Texture2D *diffuseTexture = nullptr;
	Texture2D *specularTexture = nullptr;
	RenderedTexture2D *renderedTexture = nullptr;

	FrameBuffer *frameBuffer_ = nullptr;
};

