#pragma once

class FrameBuffer
{
public:
	FrameBuffer(unsigned width = 0.0f, unsigned height = 0.0f);
	virtual ~FrameBuffer();
	void bind() const;
	void unbind() const;
	virtual void invalidate();
	void resize(unsigned width, unsigned height);
	inline unsigned int GetTextureID() const { return m_RenderTextureID; };
protected:
	unsigned int m_RendererID;
	unsigned int m_RenderTextureID;
	unsigned int m_RenderBufferID;
	unsigned int viewport_Width;
	unsigned int viewport_Height;
};