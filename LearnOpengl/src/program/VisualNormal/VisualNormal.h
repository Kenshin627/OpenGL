#include "../Shader.h"
#include "../../camera/camera.h"

class VisualNormalShader :public Shader {
public:
	VisualNormalShader(const std::vector<std::string>& paths, std::shared_ptr<Camera> cam, float magnitude, glm::vec3 lineColor);
	void setCommonUniforms() const override;
	void setMeshUniforms(std::shared_ptr<Mesh> mesh) const override;
private:
	std::shared_ptr<Camera> camera;
	float lineMagnitude;
	glm::vec3 lineColor;
};