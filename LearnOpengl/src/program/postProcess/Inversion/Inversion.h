#pragma once
#include "../PostProcess.h"

class InversionShader :public PostProcess
{
public:
	InversionShader(const std::vector<std::string>& paths, std::shared_ptr<FrameBuffer> prevbuffer);
	~InversionShader();
	void setCommonUniforms() const override;
	void unbind() const override;
};