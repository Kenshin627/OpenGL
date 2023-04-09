#include "kspch.h"
#include <glad/gl.h>
#include "ShadowFrameBuffer.h"

ShadowFrameBuffer::ShadowFrameBuffer(unsigned width, unsigned height): FrameBuffer(width, height)
{
	invalidate();
}

ShadowFrameBuffer::~ShadowFrameBuffer() { }

void ShadowFrameBuffer::invalidate()
{
	if (m_RendererID)
	{
		glDeleteFramebuffers(1, &m_RendererID);
		glDeleteTextures(1, &m_RenderTextureIDs[0]);
		glDeleteRenderbuffers(1, &m_RenderBufferID);
	}
	glGenFramebuffers(1, &m_RendererID);
	glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);

	glGenTextures(1, &m_RenderTextureIDs[0]);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_RenderTextureIDs[0]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, viewport_Width, viewport_Height, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	GLfloat borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_RenderTextureIDs[0], 0);
	glDrawBuffer(GL_NONE);
	glDrawBuffer(GL_NONE);

	glBindTexture(GL_TEXTURE_2D, 0);	


	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cout << "[ERROR]: FRAME BUFFER initialize error!";
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
