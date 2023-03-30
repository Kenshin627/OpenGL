#include "../Shader.h"

class MergeShader :public Shader {
public:
	MergeShader(const std::vector<std::string> paths, std::shared_ptr<FrameBuffer> pipelineA, std::shared_ptr<FrameBuffer> pipelineB);
	void setCommonUniforms() const override;
	void setMeshUniforms(std::shared_ptr<Mesh> mesh) const override;
private:
	std::shared_ptr<FrameBuffer> pipelineA;
	std::shared_ptr<FrameBuffer> pipelineB;
};