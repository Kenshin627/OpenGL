#pragma once
#include "../PostProcess.h"

class EdgeDetection :public PostProcess
{
public:
	EdgeDetection(const std::string& name, const std::string& vertexShaderPath, const std::string& fragmentShaderPath, PostProcessMode mode);
	~EdgeDetection();
	void draw(unsigned slot) const override;
};