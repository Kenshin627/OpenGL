#include <glad/gl.h>
#include "IndexBuffer.h"

IndexBuffer::IndexBuffer(const void* data, unsigned count):m_RendererID(0), count(count)
{
	glGenBuffers(1, &m_RendererID);
	bind();
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned) * count, data, GL_STATIC_DRAW);
	unbind();
}

IndexBuffer::~IndexBuffer()
{
	if (m_RendererID)
	{
		glDeleteBuffers(1, &m_RendererID);
	}
}

void IndexBuffer::bind() const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
}

void IndexBuffer::unbind() const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
