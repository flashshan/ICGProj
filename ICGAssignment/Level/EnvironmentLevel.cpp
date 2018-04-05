#include <GL/glew.h>
#include <GL/freeglut.h>

#include "EnvironmentLevel.h"

//#include <cyPoint.h>

#include "Core/Math/Matrix.h"
#include "Core/Math/Quaternion.h"


void EnvironmentLevel::InitLevel()
{
#pragma region Skybox VAO
	glGenVertexArrays(1, &VAOCube_);
	glBindVertexArray(VAOCube_);

	{
		const char* cubeFilePath = "cube.obj";
		if (!cube_.LoadFromFileObj(cubeFilePath))
		{
			printf_s("Load File: %s failed!\n", cubeFilePath);
			assert(0);
		}

		for (unsigned int i = 0; i < cube_.NV(); ++i)
		{
			cube_.V(i) *= 100;
		}

		GLuint VBO, IBO;

		// bind vertex data
		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, cube_.NV() * sizeof(float) * 3, &cube_.V(0), GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

		// bind index data
		glGenBuffers(1, &IBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, cube_.NF() * sizeof(unsigned int) * 3, &(cube_.F(0).v[0]), GL_STATIC_DRAW);

		const char *xposFilePath = "Data/Texture/cubemap_posx.png";
		const char *xnegFilePath = "Data/Texture/cubemap_negx.png";
		const char *yposFilePath = "Data/Texture/cubemap_posy.png";
		const char *ynegFilePath = "Data/Texture/cubemap_negy.png";
		const char *zposFilePath = "Data/Texture/cubemap_posz.png";
		const char *znegFilePath = "Data/Texture/cubemap_negz.png";

		cubeMapTexture_ = CubeMapTexture::LoadFromPath(xposFilePath, xnegFilePath, yposFilePath, ynegFilePath, zposFilePath, znegFilePath);
	}
#pragma endregion

#pragma region Sphere VAO
	glGenVertexArrays(1, &VAOSphere_);
	glBindVertexArray(VAOSphere_);

	{
		const char* sphereFilePath = "sphere.obj";
		if (!sphere_.LoadFromFileObj(sphereFilePath))
		{
			printf_s("Load File: %s failed!\n", sphereFilePath);
			assert(0);
		}

		GLuint VBO, VNO, IBO;

		// bind vertex data
		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sphere_.NV() * sizeof(float) * 3, &sphere_.V(0), GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

		glGenBuffers(1, &VNO);
		glBindBuffer(GL_ARRAY_BUFFER, VNO);
		glBufferData(GL_ARRAY_BUFFER, sphere_.NVN() * sizeof(float) * 3, &sphere_.VN(0), GL_STATIC_DRAW);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

		// bind index data
		glGenBuffers(1, &IBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sphere_.NF() * sizeof(unsigned int) * 3, &(sphere_.F(0).v[0]), GL_STATIC_DRAW);
	}
#pragma endregion

#pragma region Gen Vertex Array Teapot
	glGenVertexArrays(1, &VAOTeapot_);
	glBindVertexArray(VAOTeapot_);

	{
		const char* dataFilePath = "teapot.obj";
		if (!teapot_.LoadFromFileObj(dataFilePath))
		{
			printf_s("Load File: %s failed!\n", dataFilePath);
			assert(0);
		}

		std::vector<cy::Point3f> vData;
		vData.reserve(teapot_.NF() * 3);
		std::vector<cy::Point3f> vnData;
		vnData.reserve(teapot_.NF() * 3);

		const float additionalHeight = 20.0f;
		for (unsigned int i = 0; i < teapot_.NF(); ++i)
		{
			vData.push_back(teapot_.V(teapot_.F(i).v[0]) + cy::Point3f(0.0f, additionalHeight, 0.0f));
			vData.push_back(teapot_.V(teapot_.F(i).v[1]) + cy::Point3f(0.0f, additionalHeight, 0.0f));
			vData.push_back(teapot_.V(teapot_.F(i).v[2]) + cy::Point3f(0.0f, additionalHeight, 0.0f));
			vnData.push_back(teapot_.VN(teapot_.F(i).v[0]));
			vnData.push_back(teapot_.VN(teapot_.F(i).v[1]));
			vnData.push_back(teapot_.VN(teapot_.F(i).v[2]));
		}

		GLuint VBO, VNO;

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
	}
#pragma endregion

#pragma region Gen Vertex Array Plane
	glGenVertexArrays(1, &VAOPlane_);
	glBindVertexArray(VAOPlane_);

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

	GraphicsHelper::CompileShader("Data/Shader/Vertex/meshVertSkybox.shader", "Data/Shader/Fragment/meshFragSkybox.shader", skyBoxProgram_);
	skyBoxFTMLocation_ = glGetUniformLocation(skyBoxProgram_, "fullTransformMatrix");

	GraphicsHelper::CompileShader("Data/Shader/Vertex/meshVertReflection.shader", "Data/Shader/Fragment/meshFragReflection.shader", reflectionPrgram_);
	reflectionFTMLocation_ = glGetUniformLocation(reflectionPrgram_, "fullTransformMatrix");
	refEyePositionLocation_ = glGetUniformLocation(reflectionPrgram_, "eyePosition");
	refLightDirectionLocation_ = glGetUniformLocation(reflectionPrgram_, "lightDirection");

	GraphicsHelper::CompileShader("Data/Shader/Vertex/meshVertSimple.shader", "Data/Shader/Fragment/meshFragSimple.shader", planeProgram_);
	planeFTMLocation_ = glGetUniformLocation(planeProgram_, "fullTransformMatrix");
	renderTextureLocation_ = glGetUniformLocation(planeProgram_, "renderTexture");
#pragma endregion

	renderedTexture_ = RenderedTexture2D::MakeRenderedTexture(1000, 1000, 4.0f);

	frameBuffer_ = FrameBuffer::MakeFrameBuffer();
}



void EnvironmentLevel::DisplayLevel()
{
	Matrix fullTransformMatrix = mainCamera_->GetWorldToProjectionMatrix();
	Vector3 eyePosition = mainCamera_->GetPosition();
	Matrix fullTransformMatrix2 = mainCamera_->GetRefWorldToProjectionMatrix();
	Vector3 eyePosition2(eyePosition.X, -eyePosition.Y, eyePosition.Z);

	Quaternion lightQuat(Rotator(lightPitch_, lightYaw_, 0.0f));
	Vector3 lightDirection = lightQuat.GetAxisX();

	if (!frameBuffer_->CheckStatus())
		return;

	frameBuffer_->BindBufferTexture(renderedTexture_->GetObj());

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);

	glBindVertexArray(VAOCube_);
	glUseProgram(skyBoxProgram_);

	glUniformMatrix4fv(skyBoxFTMLocation_, 1, GL_TRUE, &fullTransformMatrix2.M[0][0]);

	cubeMapTexture_->Bind(GL_TEXTURE0);
	renderedTexture_->Bind(GL_TEXTURE1);

	glDrawElements(GL_TRIANGLES, cube_.NF() * 3, GL_UNSIGNED_INT, 0);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	glUseProgram(reflectionPrgram_);
	glBindVertexArray(VAOTeapot_);

	glUniformMatrix4fv(reflectionFTMLocation_, 1, GL_TRUE, &fullTransformMatrix2.M[0][0]);
	glUniform3fv(refEyePositionLocation_, 1, &eyePosition2.X);
	glUniform3fv(refLightDirectionLocation_, 1, &lightDirection.X);

	glDrawArrays(GL_TRIANGLES, 0, teapot_.NF() * 3);


	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
#pragma region Render Skybox
	glBindVertexArray(VAOCube_);
	glUseProgram(skyBoxProgram_);

	glUniformMatrix4fv(skyBoxFTMLocation_, 1, GL_TRUE, &fullTransformMatrix.M[0][0]);

	cubeMapTexture_->Bind(GL_TEXTURE0);

	glDrawElements(GL_TRIANGLES, cube_.NF() * 3, GL_UNSIGNED_INT, 0);
	//glDrawArrays(GL_TRIANGLES, 0, teapot.NF() * 3);
#pragma endregion

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
#pragma region Render Sphere
	//glBindVertexArray(VAOSphere);
	glUseProgram(reflectionPrgram_);

	glUniformMatrix4fv(reflectionFTMLocation_, 1, GL_TRUE, &fullTransformMatrix.M[0][0]);
	glUniform3fv(refEyePositionLocation_, 1, &eyePosition.X);
	glUniform3fv(refLightDirectionLocation_, 1, &lightDirection.X);

	//glDrawElements(GL_TRIANGLES, sphere.NF() * 3, GL_UNSIGNED_INT, 0);
#pragma endregion

#pragma region Render Teapot
	glBindVertexArray(VAOTeapot_);
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
	glDrawArrays(GL_TRIANGLES, 0, teapot_.NF() * 3);
#pragma endregion

#pragma region Render Quad
	//glDisable(GL_CULL_FACE);
	glUseProgram(planeProgram_);
	glBindVertexArray(VAOPlane_);

	//Matrix result = mainCamera.getWorldToProjectionMatrix();
	glUniformMatrix4fv(planeFTMLocation_, 1, GL_TRUE, &fullTransformMatrix.M[0][0]);

	renderedTexture_->Bind(GL_TEXTURE0);
	glUniform1i(renderTextureLocation_, 0);

	//glViewport(0, 0, 1000, 1000); // Render on the whole frame buffer, complete from the lower left corner to the upper right
	//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
#pragma endregion
}

void EnvironmentLevel::CloseLevel()
{
	glDisableVertexAttribArray(0);

	if (renderedTexture_)
		delete renderedTexture_;
	if (cubeMapTexture_)
		delete cubeMapTexture_;
}