#include "../Shader.h"
#include "../../camera/camera.h"

class EnvironmentMapRefractShader :public Shader {
public:
	EnvironmentMapRefractShader(const std::vector<std::string> paths,std::shared_ptr<Camera> cam, unsigned skyBoxTextureID, float refractIndex);
	void setCommonUniforms() const override;
	void setMeshUniforms(std::shared_ptr<Mesh> mesh) const override;
private:
	std::shared_ptr<Camera> camera;
	unsigned skyBoxTextureID;
	float refractIndex;
};