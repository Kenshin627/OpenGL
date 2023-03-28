#include "../PostProcess.h"

class GrayScaleShader :public PostProcess
{
public:
	GrayScaleShader(const std::vector<std::string>& paths, std::shared_ptr<FrameBuffer> prevbuffer);
	void setCommonUniforms() const override;
	void unbind() const override;
};