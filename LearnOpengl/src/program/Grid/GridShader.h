#include "../Shader.h"
#include "../../camera/camera.h"

class GridShader :public Shader {
public:
	GridShader(const std::vector<std::string>& paths, std::shared_ptr<Camera> cam, glm::vec3 mainCoor, glm::vec3 lineColor, glm::vec3 gridOffset, glm::vec4 gridControl);
	void setCommonUniforms() const override;
	void setMeshUniforms(std::shared_ptr<Mesh> mesh) const override;
private:
	std::shared_ptr<Camera> camera;
	glm::vec3 mainColor;
	glm::vec3 lineColor;
	glm::vec3 gridOffset;
	glm::vec4 gridControl;
};