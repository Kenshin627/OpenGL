#include "../Shader.h"
#include "../../camera/camera.h"
#include "../../light/directionLight.h"
#include "../../Buffer/ShadowFrameBuffer.h"

class GridCastShadowShader :public Shader {
public:
	GridCastShadowShader(const std::vector<std::string> paths, std::shared_ptr<Camera> cam, std::shared_ptr<DirectionLight> light, glm::vec3 mainColor, glm::vec3 lineColor, glm::vec3 gridOffset, glm::vec4 gridControl, std::shared_ptr<ShadowFrameBuffer> shadowBuffer);
	void setCommonUniforms() const override;
	void setMeshUniforms(std::shared_ptr<Mesh> mesh) const override;
private:
	std::shared_ptr<Camera> camera;
	std::shared_ptr<DirectionLight> light;
	std::shared_ptr<ShadowFrameBuffer> shadowBuffer;
	glm::vec3 mainColor;
	glm::vec3 lineColor;
	glm::vec3 gridOffset;
	glm::vec4 gridControl;
};