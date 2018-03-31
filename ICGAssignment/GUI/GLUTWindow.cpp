#include <GL/glew.h>
#include <GL/freeglut.h>


#include "GLUTWindow.h"
#include "Core/Camera.h"

#include <cyGL.h>
#include <cyTriMesh.h>
//#include <cyMatrix.h>
#include <cyPoint.h>

#include "Core/Math/Matrix.h"
#include "Core/Math/Quaternion.h"

#include "Graphics/Texture.h"

#include <stdio.h>
#include <fstream>
#include <string.h>

bool GLUTWindow::ctrlPressed_ = false;
float lightYaw = 0.0f, lightPitch = 0.0f;


cy::TriMesh teapot, cube, sphere;

GLuint skyBoxProgram, reflectionPrgram, teapotProgram, planeProgram;

// skyBox shader locations
GLuint skyBoxFTMLocation;

// reflection shader locations
GLuint reflectionFTMLocation;
GLuint refLightDirectionLocation;
GLuint refEyePositionLocation;

// teapot shader locations
GLuint teapotFTMLocation;
GLuint lightDirectionLocation;
GLuint eyePositionLocation;

//GLuint texture0Location;
//GLuint texture1Location;
//GLuint renderTextureLocation;

// quad shader locations
GLuint planeFTMLocation;
GLuint renderTextureLocation;

Vector3 lightDirection;
Vector3 eyePosition;


GLuint VAOCube, VAOSphere, VAOTeapot, VAOPlane;
Texture2D *texture1 = nullptr;
Texture2D *texture2 = nullptr;
RenderedTexture2D *renderedTexture = nullptr;

CubeMapTexture *cubeMapTexture = nullptr;

GLuint FramebufferName;

cy::GLRenderTexture<GL_TEXTURE_2D> renderedBuffer;

//Camera mainCamera(cy::Point3f(0.0f, -5.0f, 0.0f), 90.0f, 1.0f, 0.1f, 10000.0f);
Camera mainCamera(Vector3(0.0f, 30.0f, 50.0f), -180.0f, 150.0f, ProjectionParameters(90.0f, 1.0f, 0.1f, 10000.0f));

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

void CompileShader(const char* vertShaderPath, const char* fragShaderPath, GLuint &io_program)
{
	io_program = glCreateProgram();
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	std::string shaderTexts[2];
	const GLchar *shaderTextChar[2];
	GLint textLengths[2];
	shaderTexts[0] = readShaderCode(vertShaderPath);
	shaderTexts[1] = readShaderCode(fragShaderPath);
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

	glAttachShader(io_program, vertexShader);
	glAttachShader(io_program, fragmentShader);

	glLinkProgram(io_program);

	glGetProgramiv(io_program, GL_LINK_STATUS, &success);
	if (!success)
	{
		GLchar InfoLog[1024];
		glGetProgramInfoLog(io_program, sizeof(InfoLog), NULL, InfoLog);
		printf_s("Error linking shader program: '%s'\n", InfoLog);
		assert(0);
	}
}

void GLUTWindow::initOpenGL()
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

#pragma endregion

#pragma region Skybox VAO
	glGenVertexArrays(1, &VAOCube);
	glBindVertexArray(VAOCube);

	{
		const char* cubeFilePath = "cube.obj";
		if (!cube.LoadFromFileObj(cubeFilePath))
		{
			printf_s("Load File: %s failed!\n", cubeFilePath);
			assert(0);
		}

		for (int i = 0; i < cube.NV(); ++i)
		{
			cube.V(i) *= 100;
		}


		GLuint VBO, IBO;

		// bind vertex data
		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, cube.NV() * sizeof(float) * 3, &cube.V(0), GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

		// bind index data
		glGenBuffers(1, &IBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, cube.NF() * sizeof(unsigned int) * 3, &(cube.F(0).v[0]), GL_STATIC_DRAW);

		const char *xposFilePath = "Data/Texture/cubemap_posx.png";
		const char *xnegFilePath = "Data/Texture/cubemap_negx.png";
		const char *yposFilePath = "Data/Texture/cubemap_posy.png";
		const char *ynegFilePath = "Data/Texture/cubemap_negy.png";
		const char *zposFilePath = "Data/Texture/cubemap_posz.png";
		const char *znegFilePath = "Data/Texture/cubemap_negz.png";

		cubeMapTexture = CubeMapTexture::LoadFromPath(xposFilePath, xnegFilePath, yposFilePath, ynegFilePath, zposFilePath, znegFilePath);
	}
#pragma endregion

#pragma region Sphere VAO
	glGenVertexArrays(1, &VAOSphere);
	glBindVertexArray(VAOSphere);

	{
		const char* sphereFilePath = "sphere.obj";
		if (!sphere.LoadFromFileObj(sphereFilePath))
		{
			printf_s("Load File: %s failed!\n", sphereFilePath);
			assert(0);
		}

		GLuint VBO, VNO, IBO;

		// bind vertex data
		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sphere.NV() * sizeof(float) * 3, &sphere.V(0), GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

		glGenBuffers(1, &VNO);
		glBindBuffer(GL_ARRAY_BUFFER, VNO);
		glBufferData(GL_ARRAY_BUFFER, sphere.NVN() * sizeof(float) * 3, &sphere.VN(0), GL_STATIC_DRAW);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

		// bind index data
		glGenBuffers(1, &IBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sphere.NF() * sizeof(unsigned int) * 3, &(sphere.F(0).v[0]), GL_STATIC_DRAW);
	}
#pragma endregion

#pragma region Gen Vertex Array Teapot
	glGenVertexArrays(1, &VAOTeapot);
	glBindVertexArray(VAOTeapot);

	{
		const char* dataFilePath = "teapot.obj";
		if (!teapot.LoadFromFileObj(dataFilePath))
		{
			printf_s("Load File: %s failed!\n", dataFilePath);
			assert(0);
		}

		std::vector<cy::Point3f> vData;
		vData.reserve(teapot.NF() * 3);
		std::vector<cy::Point3f> vnData;
		vnData.reserve(teapot.NF() * 3);

		const float additionalHeight = 20.0f;
		for (unsigned int i = 0; i < teapot.NF(); ++i)
		{
			vData.push_back(teapot.V(teapot.F(i).v[0]) + cy::Point3f(0.0f, additionalHeight, 0.0f));
			vData.push_back(teapot.V(teapot.F(i).v[1]) + cy::Point3f(0.0f, additionalHeight, 0.0f));
			vData.push_back(teapot.V(teapot.F(i).v[2]) + cy::Point3f(0.0f, additionalHeight, 0.0f));
			vnData.push_back(teapot.VN(teapot.F(i).v[0]));
			vnData.push_back(teapot.VN(teapot.F(i).v[1]));
			vnData.push_back(teapot.VN(teapot.F(i).v[2]));
		}

		GLuint VBO, VNO, VUVO, IBO;

		// bind vertex data
		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, vData.size() * sizeof(float) * 3, &vData[0], GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

		glGenBuffers(1, &VNO);
		glBindBuffer(GL_ARRAY_BUFFER, VNO);
		glBufferData(GL_ARRAY_BUFFER, vnData.size() * sizeof(float) * 3, &vnData[0], GL_STATIC_DRAW);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

		// bind index data
		/*glGenBuffers(1, &IBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, teapot.NF() * sizeof(unsigned int) * 3, &(teapot.F(0).v[0]), GL_STATIC_DRAW);*/
	}
#pragma endregion

#pragma region Gen Vertex Array Plane
	glGenVertexArrays(1, &VAOPlane);
	glBindVertexArray(VAOPlane);

	{
		GLuint VBO2, VUVO2, IBO2;
				  
		const float halfLength = 50.0f;
		GLfloat quadVertexData[] = {
			-halfLength, 0.0f, -halfLength,
			halfLength, 0.0f, -halfLength,
			halfLength,  0.0f, halfLength,
			-halfLength,  0.0f, halfLength,
		};

		/*GLfloat quadNormalData[] = {
			0.0f, 1.0f, 0.0f,
			0.0f, 1.0f, 0.0f,
			0.0f, 1.0f, 0.0f,
			0.0f, 1.0f, 0.0f,
		};*/

		GLfloat quadUVData[] = {
			1.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f,
			1.0f, 1.0f, 0.0f,
		};

		GLuint quadIndex[] = {
			0, 2, 1, 2, 0, 3
		};

		// bind vertex data
		glGenBuffers(1, &VBO2);
		glBindBuffer(GL_ARRAY_BUFFER, VBO2);
		glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(float) * 3, &quadVertexData[0], GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		
		// bind normal data
		glGenBuffers(1, &VUVO2);
		glBindBuffer(GL_ARRAY_BUFFER, VUVO2);
		glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(float) * 3, &quadUVData[0], GL_STATIC_DRAW);
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);

		// bind index data
		glGenBuffers(1, &IBO2);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO2);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), &quadIndex[0], GL_STATIC_DRAW);
	}
#pragma endregion

#pragma region Compile shaders and get locations
	/*LoadTexture(texture1, teapot.M(0).map_Kd.data);
	LoadTexture(texture2, teapot.M(0).map_Ks.data);*/

	CompileShader("Data/Shader/Vertex/meshVertSkybox.shader", "Data/Shader/Fragment/meshFragSkybox.shader", skyBoxProgram);
	skyBoxFTMLocation = glGetUniformLocation(skyBoxProgram, "fullTransformMatrix");

	CompileShader("Data/Shader/Vertex/meshVertReflection.shader", "Data/Shader/Fragment/meshFragReflection.shader", reflectionPrgram);
	reflectionFTMLocation = glGetUniformLocation(reflectionPrgram, "fullTransformMatrix");
	refEyePositionLocation = glGetUniformLocation(reflectionPrgram, "eyePosition");
	refLightDirectionLocation = glGetUniformLocation(reflectionPrgram, "lightDirection");

	CompileShader("Data/Shader/Vertex/meshVertSimple.shader", "Data/Shader/Fragment/meshFragSimple.shader", planeProgram);
	planeFTMLocation = glGetUniformLocation(planeProgram, "fullTransformMatrix");
	renderTextureLocation = glGetUniformLocation(planeProgram, "renderTexture");
#pragma endregion

	renderedTexture = RenderedTexture2D::MakeRenderedTexture(1000, 1000, 4);
}



void GLUTWindow::display()
{

	Matrix result = mainCamera.GetWorldToProjectionMatrix();
	eyePosition = mainCamera.GetPosition();
	Matrix result2 = mainCamera.GetRefWorldToProjectionMatrix();
	Vector3 eyePosition2(eyePosition.X, -eyePosition.Y, eyePosition.Z);
	
	Quaternion lightQuat(Rotator(lightPitch, lightYaw, 0.0f));
	lightDirection = lightQuat.GetAxisX();
	
	glGenFramebuffers(1, &FramebufferName);
	glBindFramebuffer(GL_FRAMEBUFFER, FramebufferName);

	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, renderedTexture->GetObj(), 0);

	// Render to our frame buffer
	//glViewport(0, 0, 1000, 1000);  // Render on the whole frame buffer, complete from the lower left corner to the upper right

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)						 //// Always check that our frame buffer is ok
		return;

	glBindFramebuffer(GL_FRAMEBUFFER, FramebufferName);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	//// Set the list of draw buffers.
	GLenum DrawBuffers[1] = { GL_COLOR_ATTACHMENT0 };
	glDrawBuffers(1, DrawBuffers);

	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);

	glBindVertexArray(VAOCube);
	glUseProgram(skyBoxProgram);

	glUniformMatrix4fv(skyBoxFTMLocation, 1, GL_TRUE, &result2.M[0][0]);

	cubeMapTexture->Bind(GL_TEXTURE0);
	renderedTexture->Bind(GL_TEXTURE1);

	glDrawElements(GL_TRIANGLES, cube.NF() * 3, GL_UNSIGNED_INT, 0);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	glUseProgram(reflectionPrgram);
	glBindVertexArray(VAOTeapot);

	glUniformMatrix4fv(reflectionFTMLocation, 1, GL_TRUE, &result2.M[0][0]);
	glUniform3fv(refEyePositionLocation, 1, &eyePosition2.X);
	glUniform3fv(refLightDirectionLocation, 1, &lightDirection.X);

	glDrawArrays(GL_TRIANGLES, 0, teapot.NF() * 3);


	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
#pragma region Render Skybox
	glBindVertexArray(VAOCube);
	glUseProgram(skyBoxProgram);
	
	glUniformMatrix4fv(skyBoxFTMLocation, 1, GL_TRUE, &result.M[0][0]);

	cubeMapTexture->Bind(GL_TEXTURE0);
	
	glDrawElements(GL_TRIANGLES, cube.NF() * 3, GL_UNSIGNED_INT, 0);
	//glDrawArrays(GL_TRIANGLES, 0, teapot.NF() * 3);
#pragma endregion

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
#pragma region Render Sphere
	//glBindVertexArray(VAOSphere);
	glUseProgram(reflectionPrgram);

	glUniformMatrix4fv(reflectionFTMLocation, 1, GL_TRUE, &result.M[0][0]);
	glUniform3fv(refEyePositionLocation, 1, &eyePosition.X);
	glUniform3fv(refLightDirectionLocation, 1, &lightDirection.X);

	//glDrawElements(GL_TRIANGLES, sphere.NF() * 3, GL_UNSIGNED_INT, 0);
#pragma endregion

#pragma region Render Teapot
	glBindVertexArray(VAOTeapot);
	/*glUseProgram(reflectionPrgram);

	glUniformMatrix4fv(teapotFTMLocation, 1, GL_TRUE, &result.M[0][0]);
	glUniform3fv(lightDirectionLocation, 1, &lightDirection.X);
	glUniform3fv(eyePositionLocation, 1, &eyePosition.X);*/

	/*texture1->Bind(GL_TEXTURE0);
	glUniform1i(texture0Location, 0);
	texture2->Bind(GL_TEXTURE1);
	glUniform1i(texture1Location, 1);*/

	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	//glDrawElements(GL_TRIANGLES, teapot.NF() * 3, GL_UNSIGNED_INT, 0);
	glDrawArrays(GL_TRIANGLES, 0, teapot.NF() * 3);
#pragma endregion

#pragma region Render Quad
	//glDisable(GL_CULL_FACE);
	glUseProgram(planeProgram);
	glBindVertexArray(VAOPlane);

	//Matrix result = mainCamera.getWorldToProjectionMatrix();
	glUniformMatrix4fv(planeFTMLocation, 1, GL_TRUE, &result.M[0][0]);

	renderedTexture->Bind(GL_TEXTURE0);
	glUniform1i(renderTextureLocation, 0);

	//glViewport(0, 0, 1000, 1000); // Render on the whole frame buffer, complete from the lower left corner to the upper right
	//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
#pragma endregion

	glutSwapBuffers();
}

void GLUTWindow::closeOpenGL()
{
	//glValidateProgram(shaderProgram);

	glDisableVertexAttribArray(0);
	if (texture1)
		delete texture1;
	if (texture2)
		delete texture2;
	if (renderedTexture)
		delete renderedTexture;
	if (cubeMapTexture)
		delete cubeMapTexture;
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
		//CompileShader();
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

