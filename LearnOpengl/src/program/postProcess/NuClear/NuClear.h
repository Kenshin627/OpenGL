#pragma once
#include "../PostProcess.h"

class NuClearShader :public PostProcess
{
public:
	NuClearShader(const std::vector<std::string>& paths, std::shared_ptr<FrameBuffer> prevbuffer);
	~NuClearShader();
	void setCommonUniforms() const override;
	void unbind() const override;
};