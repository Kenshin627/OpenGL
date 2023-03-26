#include <glad/gl.h>
#include <iostream>
#include "ShadowFrameBuffer.h"

ShadowFrameBuffer::ShadowFrameBuffer(unsigned width, unsigned height) :viewport_Width(width), viewport_Height(height)
{
	invalidate();
}

ShadowFrameBuffer::~ShadowFrameBuffer()
{
	glDeleteFramebuffers(1, &m_RendererID);
	glDeleteTextures(1, &m_RenderTextureID);
	glDeleteRenderbuffers(1, &m_RenderBufferID);
}

void ShadowFrameBuffer::bind() const
{
	glViewport(0, 0, viewport_Width, viewport_Height);
	glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);
}

void ShadowFrameBuffer::unbind() const
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void ShadowFrameBuffer::invalidate()
{
	if (m_RendererID)
	{
		glDeleteFramebuffers(1, &m_RendererID);
		glDeleteTextures(1, &m_RenderTextureID);
		glDeleteRenderbuffers(1, &m_RenderBufferID);
	}
	glGenFramebuffers(1, &m_RendererID);
	glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);

	glGenTextures(1, &m_RenderTextureID);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_RenderTextureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, viewport_Width, viewport_Height, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_RenderTextureID, 0);
	glDrawBuffer(GL_NONE);
	glDrawBuffer(GL_NONE);

	glBindTexture(GL_TEXTURE_2D, 0);	


	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cout << "[ERROR]: FRAME BUFFER initialize error!";
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
void ShadowFrameBuffer::resize(unsigned width, unsigned height)
{
	viewport_Width = width;
	viewport_Height = height;
	invalidate();
}