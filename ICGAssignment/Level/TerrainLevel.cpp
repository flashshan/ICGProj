#include <GL/glew.h>
#include <GL/freeglut.h>

#include "TerrainLevel.h"

#include "Core/Math/Matrix.h"
#include "Core/Math/Quaternion.h"


void TerrainLevel::InitLevel()
{
#pragma region Teapot VAO
	glGenVertexArrays(1, &VAOTeapot_);
	glBindVertexArray(VAOTeapot_);

	{
		const char* dataFilePath = "Data/Mesh/Shadow/teapot.obj";
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

#pragma region Gen plane VAO
	glGenVertexArrays(1, &VAOPlane_);
	glBindVertexArray(VAOPlane_);

	{
		const float halfLength = 50.0f;

		GLfloat quadVertexData[] = {
			-halfLength, 0.0f, -halfLength,
			halfLength, 0.0f, -halfLength,
			halfLength,  0.0f, halfLength,
			-halfLength,  0.0f, halfLength,
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


#pragma region Light VAO
	glGenVertexArrays(1, &VAOLight_);
	glBindVertexArray(VAOLight_);

	{
		const char* dataFilePath = "Data/Mesh/Shadow/light.obj";
		if (!light_.LoadFromFileObj(dataFilePath))
		{
			printf_s("Load File: %s failed!\n", dataFilePath);
			assert(0);
		}

		std::vector<cy::Point3f> vData;
		vData.reserve(light_.NF() * 3);
		std::vector<cy::Point3f> vnData;
		vnData.reserve(light_.NF() * 3);

		const float additionalHeight = 30.0f;
		for (unsigned int i = 0; i < teapot_.NF(); ++i)
		{
			vData.push_back(teapot_.V(teapot_.F(i).v[0]));
			vData.push_back(teapot_.V(teapot_.F(i).v[1]));
			vData.push_back(teapot_.V(teapot_.F(i).v[2]));
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

#pragma region Terrain VAO
	terrain_ = new Terrain(0.0f, 0.0f, 200.0f, 40, 0.0f, 80.0f);
	terrain_->InitTerrain();

	glGenVertexArrays(1, &VAOTerrain_);
	glBindVertexArray(VAOTerrain_);

	{
		cy::TriMesh *terrainMesh = terrain_->GetTerrainMesh();

		GLuint VBO, VNO, VUVO, IBO;

		// bind vertex data
		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, terrainMesh->NV() * sizeof(float) * 3, &terrainMesh->V(0), GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

		glGenBuffers(1, &VNO);
		glBindBuffer(GL_ARRAY_BUFFER, VNO);
		glBufferData(GL_ARRAY_BUFFER, terrainMesh->NVN() * sizeof(float) * 3, &terrainMesh->VN(0), GL_STATIC_DRAW);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

		// bind uv data
		glGenBuffers(1, &VUVO);
		glBindBuffer(GL_ARRAY_BUFFER, VUVO);
		glBufferData(GL_ARRAY_BUFFER, terrainMesh->NVT() * sizeof(float) * 3, &terrainMesh->VT(0), GL_STATIC_DRAW);
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);

		// bind index data
		glGenBuffers(1, &IBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, terrainMesh->NF() * sizeof(unsigned int) * 3, &terrainMesh->F(0).v[0], GL_STATIC_DRAW);
	}

#pragma endregion

#pragma region Compile shader and get location
	GraphicsHelper::CompileShader("Data/Shader/Vertex/meshVertDiffuse.shader", "Data/Shader/Fragment/meshFragDiffuse.shader", terrainProgram_);
	terrainFTMLocation_ = glGetUniformLocation(terrainProgram_, "fullTransformMatrix");
	terrainDiffuseLocation_ = glGetUniformLocation(terrainProgram_, "diffuseTexture");
	terrainLightDirectionLocation_ = glGetUniformLocation(terrainProgram_, "lightDirection");

	/*GraphicsHelper::CompileShader("Data/Shader/Vertex/meshVertDepthBuffer.shader", "Data/Shader/Fragment/meshFragDepthBuffer.shader", depthProgram_);
	lightSpaceLocation_ = glGetUniformLocation(depthProgram_, "lightSpaceMatrix");

	GraphicsHelper::CompileShader("Data/Shader/Vertex/meshVert.shader", "Data/Shader/Fragment/meshFrag.shader", teapotProgram_);
	teapotFTMLocation_ = glGetUniformLocation(teapotProgram_, "fullTransformMatrix");
	lightDirectionLocation_ = glGetUniformLocation(teapotProgram_, "lightDirection");
	eyePositionLocation_ = glGetUniformLocation(teapotProgram_, "eyePosition");
	diffuseLocation_ = glGetUniformLocation(teapotProgram_, "diffuseTexture");
	specularLocation_ = glGetUniformLocation(teapotProgram_, "specularTexture");

	GraphicsHelper::CompileShader("Data/Shader/Vertex/meshVertShadow.shader", "Data/Shader/Fragment/meshFragShadow.shader", planeProgram_);
	planeFTMLocation_ = glGetUniformLocation(planeProgram_, "fullTransformMatrix");
	planeLightSpaceMatrixLocation_ = glGetUniformLocation(planeProgram_, "lightSpaceMatrix");
	depthTextureLocation_ = glGetUniformLocation(planeProgram_, "depthTexture");

	GraphicsHelper::CompileShader("Data/Shader/Vertex/meshVertLight.shader", "Data/Shader/Fragment/meshFragLight.shader", lightProgram_);
	lightPosLocation_ = glGetUniformLocation(lightProgram_, "lightPos");
	lightDiffuseLocation_ = glGetUniformLocation(lightProgram_, "diffuseColor");
	lightFTMLocation_ = glGetUniformLocation(lightProgram_, "fullTransformMatrix");

	depthTexture_ = DepthTexture2D::MakeRenderedTexture(1000, 1000); */
	terrainDiffuseTexture = Texture2D::LoadFromPath("Data/Texture/grasslight-big.png", 4.0f);
	//specularTexture = Texture2D::LoadFromPath(teapot_.M(0).map_Ks, 4.0f);
#pragma endregion

#pragma region Render teapot into texture

	//Matrix result = mainCamera_->GetWorldToProjectionMatrix();
	//glUniformMatrix4fv(normalFTMLocation_, 1, GL_TRUE, &result.M[0][0]);

	//Quaternion lightQuat(Rotator(lightPitch_, lightYaw_, 0.0f));
	//Vector3 lightDirection = lightQuat.GetAxisX();
	//Vector3 eyePosition = mainCamera_->GetPosition();

	//frameBuffer_->BindBufferTexture(renderedTexture->GetObj());

	//if (!frameBuffer_->CheckStatus())
	//	return;

	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	//glUseProgram(teapotProgram_);
	//glBindVertexArray(VAOTeapot_);

	//glUniformMatrix4fv(normalFTMLocation_, 1, GL_TRUE, &result.M[0][0]);
	//glUniform3fv(lightDirectionLocation_, 1, &lightDirection.X);
	//glUniform3fv(eyePositionLocation_, 1, &eyePosition.X);

	//diffuseTexture->Bind(GL_TEXTURE0);
	//glUniform1i(diffuseLocation_, 0);
	//specularTexture->Bind(GL_TEXTURE1);
	//glUniform1i(specularLocation_, 1);

	////glDrawElements(GL_TRIANGLES, teapot.NF() * 3, GL_UNSIGNED_INT, 0);
	//glDrawArrays(GL_TRIANGLES, 0, teapot_.NF() * 3);
#pragma endregion
}

void TerrainLevel::DisplayLevel()
{
	//Matrix lightSpaceMatrix = mainSpotLight_.GetSpotLightTransformMatrix();
	//Vector3 lightPosition = mainSpotLight_.GetPosition();
	Vector3 lightDirection = mainSpotLight_.GetDirection();

	Matrix fullTransformMatrix = mainCamera_->GetWorldToProjectionMatrix();
	//Vector3 eyePosition = mainCamera_->GetPosition();	

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	
	glUseProgram(terrainProgram_);
	glBindVertexArray(VAOTerrain_);

	glUniformMatrix4fv(terrainFTMLocation_, 1, GL_TRUE, &fullTransformMatrix.M[0][0]);
	glUniform3fv(terrainLightDirectionLocation_, 1, &lightDirection.X);

	terrainDiffuseTexture->Bind(GL_TEXTURE0);
	//glUniform1i(terrainDiffuseLocation_, 0);

	//glBindVertexArray(VAOPlane_);
	glDrawElements(GL_TRIANGLES, terrain_->GetTerrainMesh()->NF() * 3, GL_UNSIGNED_INT, 0);
	
	/*glBindVertexArray(VAOPlaneTest_);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);*/

	/*glBindVertexArray(VAOTeapot_);
	glUseProgram(teapotProgram_);
	glUniformMatrix4fv(teapotFTMLocation_, 1, GL_TRUE, &lightSpaceMatrix.M[0][0]);
	glUniform3fv(lightDirectionLocation_, 1, &lightDirection.X);
	glUniform3fv(eyePositionLocation_, 1, &eyePosition.X);

	diffuseTexture->Bind(GL_TEXTURE0);
	glUniform1i(diffuseLocation_, 0);
	specularTexture->Bind(GL_TEXTURE1);
	glUniform1i(specularLocation_, 1);

	glDrawArrays(GL_TRIANGLES, 0, teapot_.NF() * 3);

	
	glBindVertexArray(VAOLight_);
	glUseProgram(lightProgram_);

	glUniform3fv(lightPosLocation_, 1, &lightPosition.X);
	glUniform3fv(lightDiffuseLocation_, 1, &lightDiffuseColor_.X);
	glUniformMatrix4fv(lightFTMLocation_, 1, GL_TRUE, &lightSpaceMatrix.M[0][0]);

	glDrawArrays(GL_TRIANGLES, 0, light_.NF() * 3);*/
	
}

void TerrainLevel::CloseLevel()
{
	glDisableVertexAttribArray(VAOTeapot_);
	glDisableVertexAttribArray(VAOPlane_);

	//if (depthTexture_)
	//	delete depthTexture_;
	//if (depthFrameBuffer_)
	//	delete depthFrameBuffer_;

	delete terrain_;
}