#pragma once
#include "FrameBuffer.h"

class ShadowFrameBuffer:public FrameBuffer
{
public:
	ShadowFrameBuffer(unsigned width = 0.0f, unsigned height = 0.0f);
	~ShadowFrameBuffer();
	void invalidate() override;
};