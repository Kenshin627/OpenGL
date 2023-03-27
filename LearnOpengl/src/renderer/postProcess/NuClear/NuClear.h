#pragma once
#include "../PostProcess.h"

class NuClear :public PostProcess
{
public:
	NuClear(const std::string& name, const std::string& vertexShaderPath, const std::string& fragmentShaderPath, const std::string& geometryShaderPath = "");
	~NuClear();
	void draw(unsigned slot) const override;
};