#include "../Shader.h"
#include "../../camera/camera.h"

class WireFrameShader :public Shader {
public:
	WireFrameShader(const std::vector<std::string> paths, std::shared_ptr<Camera> cam, glm::vec3& wireFrameColor);
	void setCommonUniforms() const override;
	void setMeshUniforms(std::shared_ptr<Mesh> mesh) const override;
	void bind() const override;
	void unbind() const override;
private:
	std::shared_ptr<Camera> camera;
	glm::vec3& wireFrameColor;
};