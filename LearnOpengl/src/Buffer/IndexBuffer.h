#pragma once

class IndexBuffer
{
public:

	IndexBuffer(const void* data, unsigned count);
	~IndexBuffer();
	void bind() const;
	void unbind() const;
	unsigned indicesCount() const { return count; };
private:
	unsigned m_RendererID;
	unsigned count;
};