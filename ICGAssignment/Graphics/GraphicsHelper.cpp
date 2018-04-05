#pragma once

#include <GL/glew.h>
#include <GL/freeglut.h>

#include <string.h>
#include <fstream>
#include <assert.h>


namespace GraphicsHelper
{
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

};