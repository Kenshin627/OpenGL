#pragma once

class VertexBuffer
{
public:
	VertexBuffer(const void* data , unsigned size);
	~VertexBuffer();
	void bind() const;
	void unbind() const;
private:
	unsigned m_RendererID;
};