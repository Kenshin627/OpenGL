#pragma once
#include "VertexBuffer.h"
#include "VertexDataLayout.h"

class VertexArray
{
public:
	VertexArray();
	~VertexArray();
	void AddBuffer(const VertexBuffer& buffer, const VertexDataLayout& layout) const;
	void bind() const;
	void unbind() const;
private:
	unsigned m_RendererID;
};