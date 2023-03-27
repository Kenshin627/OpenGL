#pragma once

#include "../../Buffer/ShadowFrameBuffer.h"
#include "../../renderer/postProcess/PostProcess.h"
#include "../../light/directionLight.h"

class ShadowMapShader :public PostProcess {
public:
	ShadowMapShader(const std::vector<std::string>& paths, std::shared_ptr<DirectionLight> light);
	void setCommonUniforms() const override;
	void setMeshUniforms(std::shared_ptr<Mesh> mesh) const override;
	void bind() const override;
	void unbind() const override;
	void buildFBO(unsigned viewportX, unsigned viewportY) override;
private:
	std::shared_ptr<DirectionLight> light;
};