#pragma once

#include <GL/glew.h>
#include "GL/freeglut.h"

#include "Image.h"

class Texture2D {
public:
	static Texture2D* LoadFromPath(const char *i_filePath, float i_aniso)
	{
		Image *image = Image::LoadFromPath(i_filePath);

		GLuint texture1Obj;
		glGenTextures(1, &texture1Obj);

		Texture2D* res = new Texture2D(texture1Obj, image);
		glBindTexture(GL_TEXTURE_2D, res->GetObj());
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image->GetWidth(), image->GetHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, (GLvoid*)image->GetData());
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		//glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &maxAniso);
		glSamplerParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, i_aniso);

		return res;
	}

	~Texture2D()
	{
		glDeleteTextures(1, &textureObj_);
		if(sourceImage_ != nullptr)
			delete sourceImage_;
	}

	void Bind(GLenum textureUnit)
	{
		glActiveTexture(textureUnit);
		glBindTexture(GL_TEXTURE_2D, textureObj_);
	}
	
	GLuint GetObj() { return textureObj_; }

	Texture2D(GLuint i_obj, Image *sourceImage)
		: textureObj_(i_obj), sourceImage_(sourceImage)
	{
	}

private:
	GLuint textureObj_;
	Image* sourceImage_;
};

class RenderedTexture2D {
public:
	static RenderedTexture2D* MakeRenderedTexture(int width, int height, int aniso)
	{
		GLuint renderedTargetObj;
		glGenTextures(1, &renderedTargetObj);
		
		glBindTexture(GL_TEXTURE_2D, renderedTargetObj);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

		RenderedTexture2D *res = new RenderedTexture2D(renderedTargetObj);

		//glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &maxAniso);
		glSamplerParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, aniso);

		return res;
	}

	~RenderedTexture2D()
	{
		glDeleteTextures(1, &textureObj_);
	}

	void Bind(GLenum textureUnit)
	{
		//glActiveTexture(textureUnit);
		glBindTexture(GL_TEXTURE_2D, textureObj_);
	}

	GLuint GetObj() { return textureObj_; }

	RenderedTexture2D(GLuint i_obj)
		: textureObj_(i_obj)
	{
	}

private:
	GLuint textureObj_;
};


class CubeMapTexture {
public:
	static CubeMapTexture* LoadFromPath(const char *xposPath, const char *xnegPath, const char *yposPath, const char *ynegPath, const char *zposPath, const char *znegPath)
	{
		glEnable(GL_TEXTURE_CUBE_MAP);

		GLuint texture1Obj;
		glGenTextures(1, &texture1Obj);
		
		glBindTexture(GL_TEXTURE_CUBE_MAP, texture1Obj);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

		Image *posX = Image::LoadFromPath(xposPath);
		Image *negX = Image::LoadFromPath(xnegPath);
		Image *posY = Image::LoadFromPath(yposPath);
		Image *negY = Image::LoadFromPath(ynegPath);
		Image *posZ = Image::LoadFromPath(zposPath);
		Image *negZ = Image::LoadFromPath(znegPath);

		CubeMapTexture* res = new CubeMapTexture(texture1Obj, posX, negX, posY, negY, posZ, negZ);
		
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGBA, posX->GetWidth(), posX->GetHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, posX->GetData());
		glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_RGBA, negX->GetWidth(), negX->GetHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, negX->GetData());
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_RGBA, posY->GetWidth(), posY->GetHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, posY->GetData());
		glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_RGBA, negY->GetWidth(), negY->GetHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, negY->GetData());
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_RGBA, posZ->GetWidth(), posZ->GetHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, posZ->GetData());
		glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_RGBA, negZ->GetWidth(), negZ->GetHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, negZ->GetData());

		return res;
	}

	~CubeMapTexture()
	{
		glDeleteTextures(1, &textureObj_);
		delete posX_; delete negX_;
		delete posY_; delete negY_;
		delete posZ_; delete negZ_;
	}

	void Bind(GLenum textureUnit)
	{
		glActiveTexture(textureUnit);
		glBindTexture(GL_TEXTURE_CUBE_MAP, textureObj_);
	}

	GLuint GetObj() { return textureObj_; }

private:
	CubeMapTexture(GLuint i_obj, Image *posX, Image *negX, Image *posY, Image *negY, Image *posZ, Image *negZ)
		: textureObj_(i_obj), posX_(posX), negX_(negX), posY_(posY), negY_(negY), posZ_(posZ), negZ_(negZ)
	{
	}

	GLuint textureObj_;
	Image *posX_, *negX_, *posY_, *negY_, *posZ_, *negZ_;
};

