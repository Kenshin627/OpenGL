#include "../PostProcess.h"

class GrayScale :public PostProcess
{
public:
	GrayScale(const std::string& name, const std::string& vertexShaderPath, const std::string& fragmentShaderPath, PostProcessMode mode);
	void draw(unsigned slot) const override;
};