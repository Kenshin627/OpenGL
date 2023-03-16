#include <glad/gl.h>
#include "IndexBuffer.h"

IndexBuffer::IndexBuffer(const void* data, size_t count):m_RendererID(0), count(0)
{
	glGenBuffers(1, &m_RendererID);
	bind();
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned) * count, data, GL_STATIC_DRAW);
	unbind();
}

IndexBuffer::~IndexBuffer()
{
	glDeleteBuffers(1, &m_RendererID);
}

void IndexBuffer::bind() const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
}

void IndexBuffer::unbind() const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
