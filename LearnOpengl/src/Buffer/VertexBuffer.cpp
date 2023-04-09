#include "kspch.h"
#include <glad/gl.h>
#include "VertexBuffer.h"

VertexBuffer::VertexBuffer(const void* data, size_t size):m_RendererID(0)
{
	glGenBuffers(1, &m_RendererID);
	bind();
	glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
	unbind();
}

VertexBuffer::~VertexBuffer()
{
	glDeleteBuffers(1, &m_RendererID);
}

void VertexBuffer::bind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
}

void VertexBuffer::unbind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}