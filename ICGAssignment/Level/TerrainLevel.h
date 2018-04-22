#pragma once

#include "ILevel.h"

#include <cyTriMesh.h>

#include "Object/Camera.h"
#include "Object/Light.h"
#include "Object/Terrain.h"

#include "Graphics/Texture.h"
#include "Graphics/FrameBuffer.h"
#include "Graphics/GraphicsHelper.h"


class TerrainLevel : public ILevel
{
public:
	TerrainLevel(Camera *i_camera)		 
		: mainSpotLight_(Vector3(0.0f, 40.0f, 10.0f), 133.0f, -231.0f, 100.0f), mainCamera_(i_camera)
	{
	}

	virtual void InitLevel();
	virtual void DisplayLevel();
	virtual void CloseLevel();

	virtual void UpdateMainLight(float i_pitch, float i_yaw) override
	{
		mainSpotLight_.AddPitchYaw(i_pitch, i_yaw);
	}

	virtual void HandleNormalKey(char i_char) override
	{
		float moveSpeed = 0.2f;
		switch (i_char)
		{
		case 'a':
		case 'A':
			mainSpotLight_.MoveLight(-moveSpeed, 0.0f, 0.0f);
			break;
		case 'd':
		case 'D':
			mainSpotLight_.MoveLight(moveSpeed, 0.0f, 0.0f);
			break;
		case 's':
		case 'S':
			mainSpotLight_.MoveLight(0.0f, 0.0f, -moveSpeed);
			break;
		case 'w':
		case 'W':
			mainSpotLight_.MoveLight(0.0f, 0.0f, moveSpeed);
			break;
		default:
			break;
		}
	}

private:
	SpotLight mainSpotLight_;
	Camera *mainCamera_;

	Terrain *terrain_;

	cy::TriMesh teapot_, light_;
	GLuint terrainProgram_, depthProgram_, teapotProgram_, planeProgram_, lightProgram_;
	GLuint VAOTerrain_, VAOTeapot_, VAOPlane_, VAOLight_, VAOPlaneTest_;

	GLuint terrainFTMLocation_;
	GLuint terrainDiffuseLocation_;
	GLuint terrainLightDirectionLocation_;

	/*GLuint lightSpaceLocation_;

	GLuint teapotFTMLocation_;
	GLuint lightDirectionLocation_;
	GLuint eyePositionLocation_;
	GLuint diffuseLocation_;
	GLuint specularLocation_;

	GLuint planeFTMLocation_;
	GLuint planeLightSpaceMatrixLocation_;
	GLuint depthTextureLocation_;*/


	/*GLuint lightPosLocation_;
	GLuint lightDiffuseLocation_;
	GLuint lightFTMLocation_;

	Vector3 lightDiffuseColor_;*/

	Texture2D *terrainDiffuseTexture = nullptr;

	/*Texture2D *diffuseTexture = nullptr;
	Texture2D *specularTexture = nullptr;
	DepthTexture2D *depthTexture_ = nullptr;

	DepthBuffer *depthFrameBuffer_ = nullptr;*/
};

