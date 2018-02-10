#include <GL/glew.h>
#include <GL/freeglut.h>


#include "GLUTWindow.h"
#include "Core/Camera.h"

#include <cyTriMesh.h>
//#include <cyMatrix.h>
#include <cyPoint.h>

#include "Core/Math/Matrix.h"
#include "Core/Math/Quaternion.h"

#include <assert.h>
#include <stdio.h>
#include <fstream>
#include <string.h>

int GLUTWindow::bgAnimationFrame_ = 120;
int GLUTWindow::curFrame_ = 0;
bool GLUTWindow::ctrlPressed_ = false;

cy::TriMesh teapot;
GLuint shaderProgram;

//Camera mainCamera(cy::Point3f(0.0f, -5.0f, 0.0f), 90.0f, 1.0f, 0.1f, 10000.0f);
Camera mainCamera(Vector3(0.0f, -3.0f, 0.0f), -180.0f, -90.0f, ProjectionParameters(90.0f, 1.7778f, 0.1f, 10000.0f));

GLuint fullTransformMatrixLocation;
GLuint lightDirectionLocation;
GLuint eyePositionLocation;

Vector3 lightDirection;
Vector3 eyePosition;

float lightYaw = 0.0f, lightPitch = 0.0f;

GLuint VBO, VNO, IBO;

std::string readShaderCode(const char* filePath)
{
	std::ifstream input(filePath);
	if (!input.good())
	{
		printf_s("Load file %s failed:\n", filePath);
		assert(0);
	}
	return std::string(
		std::istreambuf_iterator<char>(input),
		std::istreambuf_iterator<char>());
}

void CompileShader()
{
	shaderProgram = glCreateProgram();
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	std::string shaderTexts[2];
	const GLchar *shaderTextChar[2];
	GLint textLengths[2];
	shaderTexts[0] = readShaderCode("Data/Shader/Vertex/meshVert.shader");
	shaderTexts[1] = readShaderCode("Data/Shader/Fragment/meshFrag.shader");
	shaderTextChar[0] = shaderTexts[0].c_str();
	shaderTextChar[1] = shaderTexts[1].c_str();
	textLengths[0] = static_cast<GLint>(strlen(shaderTextChar[0]));
	textLengths[1] = static_cast<GLint>(strlen(shaderTextChar[1]));

	glShaderSource(vertexShader, 1, &shaderTextChar[0], &textLengths[0]);
	glShaderSource(fragmentShader, 1, &shaderTextChar[1], &textLengths[1]);

	glCompileShader(vertexShader);
	glCompileShader(fragmentShader);

	GLint success;
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		GLchar InfoLog[1024];
		glGetShaderInfoLog(vertexShader, sizeof(InfoLog), NULL, InfoLog);
		printf_s("Error compiling shader type: %s, info: '%s'\n", "Vertex Shader", InfoLog);
		assert(0);
	}
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		GLchar InfoLog[1024];
		glGetShaderInfoLog(fragmentShader, sizeof(InfoLog), NULL, InfoLog);
		printf_s("Error compiling shader type: %s, info: '%s'\n", "Fragment Shader", InfoLog);
		assert(0);
	}

	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);

	glLinkProgram(shaderProgram);

	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success)
	{
		GLchar InfoLog[1024];
		glGetProgramInfoLog(shaderProgram, sizeof(InfoLog), NULL, InfoLog);
		printf_s("Error linking shader program: '%s'\n", InfoLog);
		assert(0);
	}

	glUseProgram(shaderProgram);
}

struct Vertex
{
	cy::Point3f Position;
	cy::Point3f Normal;
};

void GLUTWindow::initOpenGL()
{
	//glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	GLenum res = glewInit();
	if (res != GLEW_OK)
	{
		printf_s("Error: '%s'\n", glewGetErrorString(res));
		assert(0);
	}

	//std::ostream outputString(std::_Uninitialized);
	const char* dataFilePath = "Data/Mesh/teapot.obj";
	if (!teapot.LoadFromFileObj(dataFilePath, false, nullptr))
	{
		printf_s("Load File: %s failed!\n", dataFilePath);
		assert(0);
	}

	for (unsigned int i = 0; i < teapot.NV(); ++i)
	{
		teapot.V(i) *= 0.05f;
	}


	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, teapot.NV() * sizeof(float) * 3, &teapot.V(0), GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glGenBuffers(1, &VNO);
	glBindBuffer(GL_ARRAY_BUFFER, VNO);
	glBufferData(GL_ARRAY_BUFFER, teapot.NVN() * sizeof(float) * 3, &teapot.VN(0), GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	

	glGenBuffers(1, &IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, teapot.NF() * sizeof(float) * 3, &(teapot.F(0).v[0]), GL_STATIC_DRAW);
	
	
	CompileShader();

	fullTransformMatrixLocation = glGetUniformLocation(shaderProgram, "fullTransformMatrix");
	assert(fullTransformMatrixLocation != 0xFFFFFFFF);
	lightDirectionLocation = glGetUniformLocation(shaderProgram, "lightDirection");
	assert(lightDirectionLocation != 0xFFFFFFFF);
	eyePositionLocation = glGetUniformLocation(shaderProgram, "eyePosition");
	assert(eyePositionLocation != 0xFFFFFFFF);
}




void GLUTWindow::display()
{
	float alpha = (float)curFrame_ / (float)bgAnimationFrame_;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	if (++curFrame_ > bgAnimationFrame_)
		curFrame_ -= bgAnimationFrame_;

	Matrix result = mainCamera.getWorldToProjectionMatrix();
	glUniformMatrix4fv(fullTransformMatrixLocation, 1, GL_TRUE, &result.M[0][0]);

	Quaternion lightQuat(Rotator(lightPitch, lightYaw, 0.0f));
	lightDirection = lightQuat.GetAxisX();
	glUniform3fv(lightDirectionLocation, 1, &lightDirection.X);

	eyePosition = mainCamera.GetPosition();
	glUniform3fv(eyePositionLocation, 1, &eyePosition.X);

	//glDrawArrays(GL_POINTS, 0, teapot.NV());
	glDrawElements(GL_TRIANGLES, teapot.NF() * 3, GL_UNSIGNED_INT, 0);

	glutSwapBuffers();
}

void GLUTWindow::closeOpenGL()
{
	glValidateProgram(shaderProgram);
	glDisableVertexAttribArray(0);
}


void GLUTWindow::reshape(int w, int h)
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
	case 112: // key p
	case 80:  // key P
		mainCamera.SwitchView();
		break;
	}
	glutPostRedisplay();
}


void GLUTWindow::SpecialKeyPress(int key, int x, int y)
{
	switch (key)
	{
	case GLUT_KEY_F6:
		CompileShader();
		mainCamera.Reset();
	case GLUT_KEY_UP:
		mainCamera.Move(0.02f);
		break;
	case GLUT_KEY_DOWN:
		mainCamera.Move(-0.02f);
		break;
	case GLUT_KEY_CTRL_L:
		GLUTWindow::ctrlPressed_ = true;
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
		GLUTWindow::ctrlPressed_ = false;
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
		if (ctrlPressed_)
		{
			lightYaw += (x - lastMousePosX) * rotateScale;
			lightPitch += (y - lastMousePosY) * rotateScale;
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

