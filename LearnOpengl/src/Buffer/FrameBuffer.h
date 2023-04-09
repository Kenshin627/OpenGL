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
	inline unsigned int GetTextureID() const { return m_RenderTextureIDs[0]; };
protected:
	unsigned m_RendererID;
	unsigned m_RenderTextureIDs[2] = { 0, 0 };
	unsigned m_RenderBufferID;
	unsigned viewport_Width;
	unsigned viewport_Height;
};