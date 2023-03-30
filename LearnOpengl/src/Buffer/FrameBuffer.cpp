#include <glad/gl.h>
#include <iostream>
#include "FrameBuffer.h"

FrameBuffer::FrameBuffer(unsigned width, unsigned height) :viewport_Width(width), viewport_Height(height)
{
	invalidate();
}

FrameBuffer::~FrameBuffer()
{
	glDeleteFramebuffers(1, &m_RendererID);
	glDeleteTextures(2, m_RenderTextureIDs);
	glDeleteRenderbuffers(1, &m_RenderBufferID);
}

void FrameBuffer::bind() const
{
	glViewport(0, 0, viewport_Width, viewport_Height);
	glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);
}

void FrameBuffer::unbind() const
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FrameBuffer::invalidate()
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
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_RenderTextureIDs[0]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, viewport_Width, viewport_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glBindTexture(GL_TEXTURE_2D, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_RenderTextureIDs[0], 0);

	glGenRenderbuffers(1, &m_RenderBufferID);
	glBindRenderbuffer(GL_RENDERBUFFER, m_RenderBufferID);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, viewport_Width, viewport_Height);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_RenderBufferID);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cout << "[ERROR]: FRAME BUFFER initialize error!";
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
void FrameBuffer::resize(unsigned width, unsigned height)
{
	viewport_Width = width;
	viewport_Height = height;
	invalidate();
}