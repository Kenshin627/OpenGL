#include "../Shader.h"
#include "../../camera/camera.h"
#include "../../light/directionLight.h"

class BaseShader :public Shader {
public:
	BaseShader(const std::vector<std::string> paths, std::shared_ptr<Camera> cam, std::shared_ptr<DirectionLight> light);
	void setCommonUniforms() const override;
	void setMeshUniforms(std::shared_ptr<Mesh> mesh) const override;
private:
	std::shared_ptr<Camera> camera;
	std::shared_ptr<DirectionLight> light;
};