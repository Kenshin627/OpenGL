#include "../Shader.h"
#include "../../camera/camera.h"

class EnvironmentMapReflectShader :public Shader {
public:
	EnvironmentMapReflectShader(const std::vector<std::string> paths,std::shared_ptr<Camera> cam, unsigned skyBoxTextureID);
	void setCommonUniforms() const override;
	void setMeshUniforms(std::shared_ptr<Mesh> mesh) const override;
private:
	std::shared_ptr<Camera> camera;
	unsigned skyBoxTextureID;
};