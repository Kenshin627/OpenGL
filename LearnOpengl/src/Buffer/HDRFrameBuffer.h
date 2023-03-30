#pragma once
#include "FrameBuffer.h"

class HDRFrameBuffer:public FrameBuffer
{
public:
	HDRFrameBuffer(unsigned width = 0, unsigned height = 0);
	~HDRFrameBuffer();
	void invalidate() override;
	unsigned getTextureID2() const { return m_RenderTextureIDs[1]; };
};