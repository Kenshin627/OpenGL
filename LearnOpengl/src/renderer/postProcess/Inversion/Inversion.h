#pragma once
#include "../PostProcess.h"

class Inversion :public PostProcess
{
public:
	Inversion(const std::string& name, const std::string& vertexShaderPath, const std::string& fragmentShaderPath, const std::string& geometryShaderPath = "");
	~Inversion();
	void draw(unsigned slot) const override;
};