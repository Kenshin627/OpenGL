#include <glad/gl.h>
#include <iostream>
#include "HDRFrameBuffer.h"

HDRFrameBuffer::HDRFrameBuffer(unsigned width, unsigned height):FrameBuffer(width, height)
{
	invalidate();
}

HDRFrameBuffer::~HDRFrameBuffer() {}

void HDRFrameBuffer::invalidate()
{
	if (m_RendererID)
	{
		glDeleteFramebuffers(1, &m_RendererID);
		glDeleteTextures(2, m_RenderTextureIDs);
		glDeleteRenderbuffers(1, &m_RenderBufferID);
	}

	glGenFramebuffers(1, &m_RendererID);
	glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);

	glGenTextures(2, m_RenderTextureIDs);
	for (unsigned i = 0; i < 2; i++)
	{
		//glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, m_RenderTextureIDs[i]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, viewport_Width, viewport_Height, 0, GL_RGBA, GL_FLOAT, nullptr);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		//glBindTexture(GL_TEXTURE_2D, 0);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, m_RenderTextureIDs[i], 0);
	}

	glGenRenderbuffers(1, &m_RenderBufferID);
	glBindRenderbuffer(GL_RENDERBUFFER, m_RenderBufferID);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, viewport_Width, viewport_Height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_RenderBufferID);
	unsigned attachments[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
	glDrawBuffers(2, attachments);
	//glBindRenderbuffer(GL_RENDERBUFFER, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cout << "[ERROR]: FRAME BUFFER initialize error!";
	}
}