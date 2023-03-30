#pragma once
#include "FrameBuffer.h"

class ShadowFrameBuffer:public FrameBuffer
{
public:
	ShadowFrameBuffer(unsigned width = 0, unsigned height = 0);
	~ShadowFrameBuffer();
	void invalidate() override;
};