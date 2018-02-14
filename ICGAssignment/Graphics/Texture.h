#pragma once

#include <GL/glew.h>
#include "GL/freeglut.h"

class Texture {

public:
	Texture(GLuint i_target, GLuint i_obj)
		: textureTarget_(i_target), textureObj_(i_obj)
	{
	}

	void Bind(GLenum textureUnit)
	{
		glActiveTexture(textureUnit);
		glBindTexture(textureTarget_, textureObj_);
	}
	
	GLuint GetObj() { return textureObj_; }

	GLuint GetTarget() { return textureTarget_; }

private:
	GLuint textureTarget_;
	GLuint textureObj_;
};

