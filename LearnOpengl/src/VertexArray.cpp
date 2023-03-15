#include <glad/gl.h>
#include "VertexArray.h"

VertexArray::VertexArray():m_RendererID(0)
{
	glGenVertexArrays(1, &m_RendererID);
}

VertexArray::~VertexArray()
{
	glDeleteVertexArrays(1, &m_RendererID);
}

void VertexArray::AddBuffer(const VertexBuffer& buffer, const VertexDataLayout& layout) const
{
	buffer.bind();
	const VertexDataLayout::Layout_Container_Type elements = layout.getElements();
	for (size_t i = 0; i < elements.size(); i++)
	{
		const Attribute& element = elements[i];
		glVertexAttribPointer(i, element.count, element.type, element.normalized, layout.getStride(), (const void*)element.offset);
		glEnableVertexAttribArray(i);
	}
}

void VertexArray::bind() const
{
	glBindVertexArray(m_RendererID);
}

void VertexArray::unbind() const
{
	glBindVertexArray(0);
}