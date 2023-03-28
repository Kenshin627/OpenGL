#pragma once
#include "../PostProcess.h"

class EdgeDetectionShader :public PostProcess
{
public:
	EdgeDetectionShader(const std::vector<std::string>& paths, std::shared_ptr<FrameBuffer> prev_buffer);
	~EdgeDetectionShader();
	void setCommonUniforms() const override;
	void unbind() const override;
};