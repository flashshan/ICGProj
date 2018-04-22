#pragma once

#include <GL/glew.h>
#include "GL/freeglut.h"

#include "Graphics/Texture.h"
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

	void BindBufferTexture(RenderedTexture2D* i_texture2D)
	{
		glViewport(0, 0, i_texture2D->GetWidth(), i_texture2D->GetHeight()); 

		glBindFramebuffer(GL_FRAMEBUFFER, frameBufferObj_);
		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, i_texture2D->GetObj(), 0);
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
};



class DepthBuffer {
public:
	static DepthBuffer* MakeDepthBuffer()
	{
		GLuint depthBufferObj;
		glGenFramebuffers(1, &depthBufferObj);

		////// Set the list of draw buffers.
		//GLenum DrawBuffers[1] = { GL_DEPTH_ATTACHMENT };
		//glDrawBuffers(1, DrawBuffers);

		DepthBuffer *res = new DepthBuffer(depthBufferObj);
		return res;
	}

	void BindBufferTexture(DepthTexture2D* i_texture2D)
	{
		glViewport(0, 0, i_texture2D->GetWidth(), i_texture2D->GetHeight());

		glBindFramebuffer(GL_FRAMEBUFFER, depthBufferObj_);	
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, i_texture2D->GetObj(), 0);
		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);
	}

	bool CheckStatus()
	{
		return glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE;
	}

private:
	DepthBuffer(GLuint i_depthBufferObj)
		: depthBufferObj_(i_depthBufferObj)
	{
	}

	GLuint depthBufferObj_;
};