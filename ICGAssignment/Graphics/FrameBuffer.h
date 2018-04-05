#pragma once

#include <GL/glew.h>
#include "GL/freeglut.h"

#include <assert.h>

class FrameBuffer {
public:
	static FrameBuffer* MakeFrameBuffer()
	{
		GLuint frameBufferObj;
		glGenFramebuffers(1, &frameBufferObj);
		
		//// Set the list of draw buffers.
		GLenum DrawBuffers[1] = { GL_COLOR_ATTACHMENT0 };
		glDrawBuffers(1, DrawBuffers);

		FrameBuffer *res = new FrameBuffer(frameBufferObj);
		return res;
	}

	void BindBufferTexture(GLuint i_textureObj)
	{
		glViewport(0, 0, width_, height_); 

		glBindFramebuffer(GL_FRAMEBUFFER, frameBufferObj_);
		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, i_textureObj, 0);
	}

	bool CheckStatus()
	{
		return glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE;
	}

private:
	FrameBuffer(GLuint i_frameBufferObj)
		: frameBufferObj_(i_frameBufferObj)
	{
	}

	GLuint frameBufferObj_;
	unsigned int width_, height_;
};

