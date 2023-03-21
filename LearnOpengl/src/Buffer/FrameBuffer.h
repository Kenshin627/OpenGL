#pragma once

class FrameBuffer
{
public:
	FrameBuffer(unsigned width, unsigned height);
	~FrameBuffer();
	void bind() const;
	void unbind() const;
	void TextureAttachment();
	void RenderBufferAttachment();
	inline unsigned int GetTextureID() const { return m_RenderTextureID; };
private:
	unsigned int m_RendererID;
	unsigned int m_RenderTextureID;
	unsigned int m_RenderBufferID;
	unsigned int viewport_Width;
	unsigned int viewport_Height;
};