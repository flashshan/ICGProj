#include <GL/glew.h>
#include <GL/freeglut.h>

#include "RenderTextureLevel.h"


#include "Core/Math/Matrix.h"
#include "Core/Math/Quaternion.h"


void RenderTextureLevel::InitLevel()
{
#pragma region Teapot VAO
	glGenVertexArrays(1, &VAOTeapot_);
	glBindVertexArray(VAOTeapot_);

	{
		const char* dataFilePath = "Data/Mesh/RenderTeapot/teapot.obj";
		if (!teapot_.LoadFromFileObj(dataFilePath))
		{
			printf_s("Load File: %s failed!\n", dataFilePath);
			assert(0);
		}

		std::vector<cy::Point3f> vData;
		vData.reserve(teapot_.NF() * 3);
		std::vector<cy::Point3f> vnData;
		vnData.reserve(teapot_.NF() * 3);
		std::vector<cy::Point3f> uvData;
		uvData.reserve(teapot_.NF() * 3);

		const float additionalHeight = 24.0f;
		for (unsigned int i = 0; i < teapot_.NF(); ++i)
		{
			vData.push_back(teapot_.V(teapot_.F(i).v[0]) + cy::Point3f(0.0f, additionalHeight, 0.0f));
			vData.push_back(teapot_.V(teapot_.F(i).v[1]) + cy::Point3f(0.0f, additionalHeight, 0.0f));
			vData.push_back(teapot_.V(teapot_.F(i).v[2]) + cy::Point3f(0.0f, additionalHeight, 0.0f));
			vnData.push_back(teapot_.VN(teapot_.F(i).v[0]));
			vnData.push_back(teapot_.VN(teapot_.F(i).v[1]));
			vnData.push_back(teapot_.VN(teapot_.F(i).v[2]));
			uvData.push_back(teapot_.VT(teapot_.FT(i).v[0]));
			uvData.push_back(teapot_.VT(teapot_.FT(i).v[1]));
			uvData.push_back(teapot_.VT(teapot_.FT(i).v[2]));
		}

		GLuint VBO, VNO, VUVO;

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

		glGenBuffers(1, &VUVO);
		glBindBuffer(GL_ARRAY_BUFFER, VUVO);
		glBufferData(GL_ARRAY_BUFFER, uvData.size() * sizeof(float) * 3, &uvData[0], GL_STATIC_DRAW);
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);
	}

#pragma endregion

#pragma region Gen plane VAO
	glGenVertexArrays(1, &VAOPlane_);
	glBindVertexArray(VAOPlane_);

	{
		Vector3 planePos(0.0f, 1.0f, 30.0f);
		const float halfLength = 50.0f;

		GLfloat quadVertexData[] = {
			-halfLength + planePos.X, 0.0f + planePos.Y, -halfLength + planePos.Z,
			halfLength + planePos.X, 0.0f + planePos.Y, -halfLength + planePos.Z,
			halfLength + planePos.X,  0.0f + planePos.Y, halfLength + planePos.Z,
			-halfLength + planePos.X,  0.0f + planePos.Y, halfLength + planePos.Z,
		};

		GLfloat quadUVData[] = {
			1.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f,
			1.0f, 1.0f, 0.0f,
		};

		GLuint quadIndex[] = {
			0, 2, 1, 2, 0, 3
		};

		GLuint VBO, VUVO, IBO;

		// bind vertex data
		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(float) * 3, &quadVertexData[0], GL_STATIC_DRAW);
		//glBufferData(GL_ARRAY_BUFFER, teapot.NV() * sizeof(float) * 3, &teapot.V(0), GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

		// bind uv data
		glGenBuffers(1, &VUVO);
		glBindBuffer(GL_ARRAY_BUFFER, VUVO);
		glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(float) * 3, &quadUVData[0], GL_STATIC_DRAW);
		//glBufferData(GL_ARRAY_BUFFER, teapot.NVT() * sizeof(float) * 3, &teapot.VT(0), GL_STATIC_DRAW);
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);

		// bind index data
		glGenBuffers(1, &IBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), &quadIndex[0], GL_STATIC_DRAW);
		//glBufferData(GL_ELEMENT_ARRAY_BUFFER, teapot.NF() * sizeof(unsigned int) * 3, &(teapot.F(0).v[0]), GL_STATIC_DRAW);
	}
#pragma endregion

#pragma region Compile shader and get location
	GraphicsHelper::CompileShader("Data/Shader/Vertex/meshVert.shader", "Data/Shader/Fragment/meshFrag.shader", teapotProgram_);

	// assert for test
	normalFTMLocation_ = glGetUniformLocation(teapotProgram_, "fullTransformMatrix");
	lightDirectionLocation_ = glGetUniformLocation(teapotProgram_, "lightDirection");
	eyePositionLocation_ = glGetUniformLocation(teapotProgram_, "eyePosition");
	diffuseLocation_ = glGetUniformLocation(teapotProgram_, "diffuseTexture");
	specularLocation_ = glGetUniformLocation(teapotProgram_, "specularTexture");

	// Create and compile our GLSL program from the shaders
	GraphicsHelper::CompileShader("Data/Shader/Vertex/meshVertSimple.shader", "Data/Shader/Fragment/meshFragSimple.shader", planeProgram_);

	planeFTMLocation_ = glGetUniformLocation(planeProgram_, "fullTransformMatrix");
	//assert(fullTransformMatrixLocation2 != 0xFFFFFFFF);
	renderedTextureLocation_ = glGetUniformLocation(planeProgram_, "renderedTexture");
	//assert(renderedTextureLocation != 0xFFFFFFFF);

	diffuseTexture = Texture2D::LoadFromPath(teapot_.M(0).map_Kd.data, 4.0f);
	specularTexture = Texture2D::LoadFromPath(teapot_.M(0).map_Ks.data, 4.0f);
	renderedTexture = RenderedTexture2D::MakeRenderedTexture(1000, 1000, 4.0f);

	frameBuffer_ = FrameBuffer::MakeFrameBuffer();
#pragma endregion

#pragma region Render teapot into texture

	Matrix fullTransformMatrix = mainCamera_->GetWorldToProjectionMatrix();
	glUniformMatrix4fv(normalFTMLocation_, 1, GL_TRUE, &fullTransformMatrix.M[0][0]);

	Vector3 lightDirection = mainLight_.GetDirection();
	Vector3 eyePosition = mainCamera_->GetPosition();

	frameBuffer_->BindBufferTexture(renderedTexture);

	if (!frameBuffer_->CheckStatus())
		return;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	glUseProgram(teapotProgram_);
	glBindVertexArray(VAOTeapot_);

	glUniformMatrix4fv(normalFTMLocation_, 1, GL_TRUE, &fullTransformMatrix.M[0][0]);
	glUniform3fv(lightDirectionLocation_, 1, &lightDirection.X);
	glUniform3fv(eyePositionLocation_, 1, &eyePosition.X);

	diffuseTexture->Bind(GL_TEXTURE0);
	glUniform1i(diffuseLocation_, 0);
	specularTexture->Bind(GL_TEXTURE1);
	glUniform1i(specularLocation_, 1);

	//glDrawElements(GL_TRIANGLES, teapot.NF() * 3, GL_UNSIGNED_INT, 0);
	glDrawArrays(GL_TRIANGLES, 0, teapot_.NF() * 3);
#pragma endregion
}

void RenderTextureLevel::DisplayLevel()
{
	Matrix fullTransformMatrix = mainCamera_->GetWorldToProjectionMatrix();

	//Quaternion lightQuat(Rotator(lightPitch_, lightYaw_, 0.0f));
	//Vector3 lightDirection = lightQuat.GetAxisX();
	//Vector3 eyePosition = mainCamera_->GetPosition();

	//glUseProgram(teapotProgram_);
	//glBindVertexArray(VAOTeapot_);

	//glUniformMatrix4fv(normalFTMLocation_, 1, GL_TRUE, &fullTransformMatrix.M[0][0]);
	//glUniform3fv(lightDirectionLocation_, 1, &lightDirection.X);
	//glUniform3fv(eyePositionLocation_, 1, &eyePosition.X);

	//diffuseTexture->Bind(GL_TEXTURE0);
	//glUniform1i(diffuseLocation_, 0);
	//specularTexture->Bind(GL_TEXTURE1);
	//glUniform1i(specularLocation_, 1);

	////glDrawElements(GL_TRIANGLES, teapot.NF() * 3, GL_UNSIGNED_INT, 0);
	//glDrawArrays(GL_TRIANGLES, 0, teapot_.NF() * 3);

	//glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glBindVertexArray(VAOPlane_);
	glUseProgram(planeProgram_);

	glUniformMatrix4fv(planeFTMLocation_, 1, GL_TRUE, &fullTransformMatrix.M[0][0]);

	renderedTexture->Bind(GL_TEXTURE0);
	glUniform1i(renderedTextureLocation_, 0);


	glDrawElements(GL_TRIANGLES, /*teapot.NF() * */6, GL_UNSIGNED_INT, 0);
}

void RenderTextureLevel::CloseLevel()
{
	glDisableVertexAttribArray(VAOTeapot_);
	glDisableVertexAttribArray(VAOPlane_);

	if (diffuseTexture)
		delete diffuseTexture;
	if (specularTexture)
		delete specularTexture;
	if (renderedTexture)
		delete renderedTexture;
	if (frameBuffer_)
		delete frameBuffer_;
}