#pragma once

#include <GL/glew.h>


namespace GraphicsHelper
{
	void CompileShader(const char* vertShaderPath, const char* fragShaderPath, GLuint &io_program);

};