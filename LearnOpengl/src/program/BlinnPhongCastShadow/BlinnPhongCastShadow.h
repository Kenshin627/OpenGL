#include "../Shader.h"
#include "../../camera/camera.h"
#include "../../light/directionLight.h"
#include "../../Buffer/ShadowFrameBuffer.h"

class BlinnPhongCastShadowShader :public Shader {
public:
	BlinnPhongCastShadowShader(const std::vector<std::string> paths, std::shared_ptr<Camera> cam, std::shared_ptr<DirectionLight> light, std::shared_ptr<ShadowFrameBuffer> shadowFrameBuffer);
	void setCommonUniforms() const override;
	void setMeshUniforms(std::shared_ptr<Mesh> mesh) const override;
private:
	std::shared_ptr<Camera> camera;
	std::shared_ptr<DirectionLight> light;
	std::shared_ptr<ShadowFrameBuffer> shadowFrameBuffer;
};