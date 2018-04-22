#include <GL/glew.h>
#include <GL/freeglut.h>

#include <cstdlib>

#include "GLUTWindow.h"

#include <cyGL.h>
#include <cyTriMesh.h>
//#include <cyMatrix.h>
#include <cyPoint.h>

#include "Core/Math/Matrix.h"
#include "Core/Math/Quaternion.h"

#include "Level/RenderTextureLevel.h"
#include "Level/EnvironmentLevel.h"
#include "Level/ShadowLevel.h"
#include "Level/ShadowLevelTest.h"
#include "Level/TerrainLevel.h"

bool ctrlPressed = false;

//Camera mainCamera(Vector3(0.0f, 30.0f, 50.0f), -180.0f, 150.0f, ProjectionParameters(90.0f, 1.0f, 0.1f, 10000.0f));
Camera mainCamera(Vector3(0.0f, 70.0f, 50.0f), -180.0f, 150.0f, ProjectionParameters(90.0f, 1.0f, 0.1f, 100.0f));

std::vector<ILevel *> levels;

// temporary method
void switchToLevel(ILevel *i_level)
{
	for (unsigned int i = 0; i < levels.size(); ++i)
	{
		levels[i]->CloseLevel();
		delete levels[i];
	}
	levels.clear();

	levels.push_back(i_level);
	for (unsigned int i = 0; i < levels.size(); ++i)
		levels[i]->InitLevel();
}


void GLUTWindow::InitOpenGL()
{
#pragma region Init glew
	GLenum res = glewInit();
	if (res != GLEW_OK)
	{
		printf_s("Error: '%s'\n", glewGetErrorString(res));
		assert(0);
	}

	glFrontFace(GL_CW);
	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
#pragma endregion

	//levels.push_back(new EnvironmentLevel(&mainCamera));
	//levels.push_back(new RenderTextureLevel(&mainCamera));

	//levels.push_back(new ShadowLevel(&mainCamera));
	//levels.push_back(new ShadowLevelTest(&mainCamera));
	
	switchToLevel(new ShadowLevelTest(&mainCamera));
	//switchToLevel(new TerrainLevel(&mainCamera));
}



void GLUTWindow::Display()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	for (unsigned int i = 0; i < levels.size(); ++i)
		levels[i]->DisplayLevel();

	glutSwapBuffers();
}

void GLUTWindow::CloseOpenGL()
{
	glDisableVertexAttribArray(0);

	for (unsigned int i = 0; i < levels.size(); ++i)
		levels[i]->CloseLevel();
}


void GLUTWindow::Reshape(int w, int h)
{
	glViewport(0, 0, w, h);
}

void GLUTWindow::NormalKeyPress(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 27: // Escape key
		exit(0);
		break;
	case 'P': 
	case 'p': 
		mainCamera.SwitchView();
		break;
	case 'A':  // key A
	case 'a':  // key a
	case 'D':  // key D
	case 'd': // key d
	case 'W':  // key D
	case 'w': // key d
	case 'S':  // key D
	case 's': // key d
		for (unsigned int i = 0; i < levels.size(); ++i)
			levels[i]->HandleNormalKey(key);
		break;
	case '1':
		switchToLevel(new RenderTextureLevel(&mainCamera));
		break;
	case '2':
		switchToLevel(new EnvironmentLevel(&mainCamera));
		break;
	case '3':
		switchToLevel(new ShadowLevel(&mainCamera));
		break;
	case '4':
		switchToLevel(new TerrainLevel(&mainCamera));
		break;
		
	default:
		break;
	}
	glutPostRedisplay();
}


void GLUTWindow::SpecialKeyPress(int key, int x, int y)
{
	switch (key)
	{
	case GLUT_KEY_F6:
		mainCamera.Reset();
	case GLUT_KEY_UP:
		mainCamera.Move(0.02f);
		break;
	case GLUT_KEY_DOWN:
		mainCamera.Move(-0.02f);
		break;
	case GLUT_KEY_CTRL_L:
		ctrlPressed = true;
		break;
	default:
		break;
	}
}

void GLUTWindow::SpecialKeyRelease(int key, int x, int y)
{
	switch (key)
	{
	case GLUT_KEY_CTRL_L:
		ctrlPressed = false;
		break;
	default:
		break;
	}
}

float lastMousePosX;
float lastMousePosY;

bool leftButtonDown = false, rightButtonDown = false;


void GLUTWindow::PassiveMouseCB(int x, int y)
{
	/*if (leftButtonDown)
	{
		float rotateScale = 0.1f;
		mainCamera.Rotate((x - lastMousePosX) * rotateScale, (y - lastMousePosY) * rotateScale);
	}
	if (rightButtonDown)
	{
		float moveScale = 0.05f;
		mainCamera.Rotate((x - lastMousePosX) * moveScale, (y - lastMousePosY) * moveScale);
	}*/

	float maxDistance = 5.0f;

	if (x - lastMousePosX > -maxDistance && x - lastMousePosX < maxDistance &&
		y - lastMousePosY > -maxDistance && y - lastMousePosY < maxDistance)
	{
		float rotateScale = 1.0f;
		if (ctrlPressed)
		{
			for (unsigned int i = 0; i < levels.size(); ++i)
				levels[i]->UpdateMainLight((x - lastMousePosX) * rotateScale, (y - lastMousePosY) * rotateScale);
		}
		else
		{
			mainCamera.Rotate((x - lastMousePosX) * rotateScale, (y - lastMousePosY) * rotateScale);
		}
	}
	lastMousePosX = float(x);
	lastMousePosY = float(y);
}



void GLUTWindow::MouseButtonPress(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON)
	{
		if (state == GLUT_DOWN)
		{
			if (!rightButtonDown)
			{
				leftButtonDown = true;
				lastMousePosX = (float)x;
				lastMousePosY = (float)y;
			}
		}
		else
		{
			leftButtonDown = false;
		}
	}
	if (button == GLUT_RIGHT_BUTTON)
	{
		if (state == GLUT_DOWN)
		{
			if (!leftButtonDown)
			{
				rightButtonDown = true;
				lastMousePosX = (float)x;
				lastMousePosY = (float)y;
			}
		}
		else
		{
			rightButtonDown = false;
		}
	}
}

