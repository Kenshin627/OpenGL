#include "../PostProcess.h"

class GrayScale :public PostProcess
{
public:
	GrayScale(const std::string& name, const std::string& vertexShaderPath, const std::string& fragmentShaderPath, const std::string& geometryShaderPath = "");
	void draw(unsigned slot) const override;
};