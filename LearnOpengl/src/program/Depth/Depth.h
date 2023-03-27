#include "../Shader.h"
#include "../../camera/camera.h"

class DepthShader :public Shader {
public:
	DepthShader(const std::vector<std::string> paths, std::shared_ptr<Camera> cam);
	void setCommonUniforms() const override;
	void setMeshUniforms(std::shared_ptr<Mesh> mesh) const override;
private:
	std::shared_ptr<Camera> camera;
};