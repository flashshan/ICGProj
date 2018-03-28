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

#include "Graphics/ImageLoader/lodepng.h"
#include "Graphics/Texture.h"

#include <assert.h>
#include <stdio.h>
#include <fstream>
#include <string.h>

bool GLUTWindow::ctrlPressed_ = false;

cy::TriMesh teapot;
GLuint firstProgram, secondProgram;

//Camera mainCamera(cy::Point3f(0.0f, -5.0f, 0.0f), 90.0f, 1.0f, 0.1f, 10000.0f);
Camera mainCamera(Vector3(0.0f, -20.0f, 20.0f), -180.0f, -135.0f, ProjectionParameters(90.0f, 1.0f, 0.1f, 10000.0f));

GLuint fullTransformMatrixLocation1;
GLuint lightDirectionLocation;
GLuint eyePositionLocation;
GLuint texture0Location;
GLuint texture1Location;

GLuint renderTextureLocation;
GLuint fullTransformMatrixLocation2;

Vector3 lightDirection;
Vector3 eyePosition;

float lightYaw = 0.0f, lightPitch = 0.0f;

GLuint VAO, VAO2;
GLuint VBO, VNO, VUVO, IBO;
GLuint VBO2, VUVO2, IBO2;
Texture *texture1 = nullptr;
Texture *texture2 = nullptr;
Texture *renderedTexture = nullptr;

GLuint FramebufferName;

cy::GLRenderTexture<GL_TEXTURE_2D> renderedBuffer;

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

void LoadTexture(Texture *&io_texture, const char *i_filePath)
{
	unsigned char *data = nullptr;
	unsigned int width, height;
	//lodepng_decode_file(data, &width, &height, "Data/Texture/yoda-body.png", LodePNGColorType::LCT_RGBA, 8);
	if (lodepng_decode32_file(&data, &width, &height, i_filePath))
	{
		printf_s("Load tex0 error!\n");
		assert(0);
	}

	GLuint texture1Obj;
	glGenTextures(1, &texture1Obj);
	if (io_texture != nullptr)
		delete io_texture;

	io_texture = new Texture(GL_TEXTURE_2D, texture1Obj);
	glBindTexture(io_texture->GetTarget(), io_texture->GetObj());
	glTexImage2D(io_texture->GetTarget(), 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, (GLvoid*)data);
	glTexParameterf(io_texture->GetTarget(), GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(io_texture->GetTarget(), GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &maxAniso);
	glSamplerParameterf(io_texture->GetTarget(), GL_TEXTURE_MAX_ANISOTROPY_EXT, 6.0f);
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

	//std::ostream outputString(std::_Uninitialized);
	const char* dataFilePath = "teapot.obj";
	if (!teapot.LoadFromFileObj(dataFilePath))
	{
		printf_s("Load File: %s failed!\n", dataFilePath);
		assert(0);
	}

	glFrontFace(GL_CW);
	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);

#pragma endregion

#pragma region Gen Vertex Array 1
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	std::vector<cy::Point3f> vData;
	vData.reserve(teapot.NF() * 3);
	std::vector<cy::Point3f> vnData;
	vnData.reserve(teapot.NF() * 3);
	std::vector<cy::Point3f> uvData;
	uvData.reserve(teapot.NF() * 3);
		
	for (unsigned int i = 0; i < teapot.NF(); ++i)
	{
		vData.push_back(teapot.V(teapot.F(i).v[0]));
		vData.push_back(teapot.V(teapot.F(i).v[1]));
		vData.push_back(teapot.V(teapot.F(i).v[2]));
		vnData.push_back(teapot.VN(teapot.F(i).v[0]));
		vnData.push_back(teapot.VN(teapot.F(i).v[1]));
		vnData.push_back(teapot.VN(teapot.F(i).v[2]));
		uvData.push_back(teapot.VT(teapot.FT(i).v[0]));
		uvData.push_back(teapot.VT(teapot.FT(i).v[1]));
		uvData.push_back(teapot.VT(teapot.FT(i).v[2]));
	}

	// bind vertex data
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vData.size() * sizeof(float) * 3, &vData[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	// bind normal data
	glGenBuffers(1, &VNO);
	glBindBuffer(GL_ARRAY_BUFFER, VNO);
	glBufferData(GL_ARRAY_BUFFER, vnData.size() * sizeof(float) * 3, &vnData[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

	// bind uv data
	glGenBuffers(1, &VUVO);
	glBindBuffer(GL_ARRAY_BUFFER, VUVO);
	glBufferData(GL_ARRAY_BUFFER,  uvData.size() * sizeof(float) * 3, &uvData[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);

	// bind index data
	/*glGenBuffers(1, &IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, teapot.NF() * sizeof(unsigned int) * 3, &(teapot.F(0).v[0]), GL_STATIC_DRAW);*/
	
#pragma endregion

#pragma region Compile shader and get location
	LoadTexture(texture1, teapot.M(0).map_Kd.data);
	LoadTexture(texture2, teapot.M(0).map_Ks.data);

	CompileShader("Data/Shader/Vertex/meshVert.shader", "Data/Shader/Fragment/meshFrag.shader", firstProgram);

	// assert for test
	fullTransformMatrixLocation1 = glGetUniformLocation(firstProgram, "fullTransformMatrix");
	//assert(fullTransformMatrixLocation != 0xFFFFFFFF);
	lightDirectionLocation = glGetUniformLocation(firstProgram, "lightDirection");
	//assert(lightDirectionLocation != 0xFFFFFFFF);
	eyePositionLocation = glGetUniformLocation(firstProgram, "eyePosition");
	//assert(eyePositionLocation != 0xFFFFFFFF);
	texture0Location = glGetUniformLocation(firstProgram, "texSampler0");
	//assert(texture0Location != 0xFFFFFFFF);
	texture1Location = glGetUniformLocation(firstProgram, "texSampler1");
	//assert(texture1Location != 0xFFFFFFFF);
#pragma endregion

#pragma region Render teapot into texture
	GLuint renderedTargetObj;
	glGenTextures(1, &renderedTargetObj);
	renderedTexture = new Texture(GL_TEXTURE_2D, renderedTargetObj);
	glBindTexture(renderedTexture->GetTarget(), renderedTexture->GetObj());
	glTexImage2D(renderedTexture->GetTarget(), 0, GL_RGBA, 1280, 720, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glGenFramebuffers(1, &FramebufferName);
	glBindFramebuffer(GL_FRAMEBUFFER, FramebufferName);
	//// Set "renderedTexture" as our color attachment #0
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, renderedTexture->GetObj(), 0);

	//// Set the list of draw buffers.
	GLenum DrawBuffers[1] = { GL_COLOR_ATTACHMENT0 };
	glDrawBuffers(1, DrawBuffers); 
	//
	//// Always check that our frame buffer is ok
	//if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	//	return;

	glUseProgram(firstProgram);
	glBindVertexArray(VAO);

	Matrix result = mainCamera.getWorldToProjectionMatrix();
	glUniformMatrix4fv(fullTransformMatrixLocation1, 1, GL_TRUE, &result.M[0][0]);

	Quaternion lightQuat(Rotator(lightPitch, lightYaw, 0.0f));
	lightDirection = lightQuat.GetAxisX();
	glUniform3fv(lightDirectionLocation, 1, &lightDirection.X);

	eyePosition = mainCamera.GetPosition();
	glUniform3fv(eyePositionLocation, 1, &eyePosition.X);

	texture1->Bind(GL_TEXTURE0);
	glUniform1i(texture0Location, 0);
	texture2->Bind(GL_TEXTURE1);
	glUniform1i(texture1Location, 1);

	// Render to our frame buffer
	glViewport(0, 0, 1280, 720);  // Render on the whole frame buffer, complete from the lower left corner to the upper right
						 
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)						 //// Always check that our frame buffer is ok
		return;

	glBindFramebuffer(GL_FRAMEBUFFER, FramebufferName);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	//glDrawElements(GL_TRIANGLES, teapot.NF() * 3, GL_UNSIGNED_INT, 0);
	glDrawArrays(GL_TRIANGLES, 0, vData.size());
#pragma endregion

#pragma region Gen Vertex Array 2
	glGenVertexArrays(1, &VAO2);
	glBindVertexArray(VAO2);

	GLfloat quadVertexData[] = {
		-20.0f, 0.0f, -20.0f, 
		20.0f, 0.0f, -20.0f,
		20.0f,  0.0f, 20.0f, 
		-20.0f,  0.0f, 20.0f,
	};

	GLfloat quadUVData[] = {
		0.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
	};

	GLuint quadIndex[] = {
		0, 1, 2, 2, 3, 0
	};

	// bind vertex data
	glGenBuffers(1, &VBO2);
	glBindBuffer(GL_ARRAY_BUFFER, VBO2);
	glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(float) * 3, &quadVertexData[0], GL_STATIC_DRAW);
	//glBufferData(GL_ARRAY_BUFFER, teapot.NV() * sizeof(float) * 3, &teapot.V(0), GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);


	// bind uv data
	glGenBuffers(1, &VUVO2);
	glBindBuffer(GL_ARRAY_BUFFER, VUVO2);
	glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(float) * 3, &quadUVData[0], GL_STATIC_DRAW);
	//glBufferData(GL_ARRAY_BUFFER, teapot.NVT() * sizeof(float) * 3, &teapot.VT(0), GL_STATIC_DRAW);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);

	// bind index data
	glGenBuffers(1, &IBO2);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO2);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), &quadIndex[0], GL_STATIC_DRAW);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, teapot.NF() * sizeof(unsigned int) * 3, &(teapot.F(0).v[0]), GL_STATIC_DRAW);
#pragma endregion

#pragma region Compile another shader and get locations
	// Create and compile our GLSL program from the shaders
	CompileShader("Data/Shader/Vertex/meshVertSimple.shader", "Data/Shader/Fragment/meshFragSimple.shader", secondProgram);

	glUseProgram(secondProgram);

	renderTextureLocation = glGetUniformLocation(secondProgram, "renderTexture");
	assert(renderTextureLocation != 0xFFFFFFFF);
	fullTransformMatrixLocation2 = glGetUniformLocation(secondProgram, "fullTransformMatrix");
	assert(fullTransformMatrixLocation2 != 0xFFFFFFFF);
#pragma endregion
}



void GLUTWindow::display()
{

	Matrix result = mainCamera.getWorldToProjectionMatrix();

	glBindVertexArray(VAO2);

	//Matrix result = mainCamera.getWorldToProjectionMatrix();
	glUniformMatrix4fv(fullTransformMatrixLocation2, 1, GL_TRUE, &result.M[0][0]);

	renderedTexture->Bind(GL_TEXTURE0);
	glUniform1i(renderTextureLocation, 0);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glViewport(0, 0, 1280, 720); // Render on the whole frame buffer, complete from the lower left corner to the upper right
	//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glDrawElements(GL_TRIANGLES, /*teapot.NF() * */6, GL_UNSIGNED_INT, 0);

	glutSwapBuffers();
}

void GLUTWindow::closeOpenGL()
{
	//glValidateProgram(shaderProgram);
	glDisableVertexAttribArray(0);
	delete texture1;
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

