#pragma once

#include "../Shader.h"
#include "../../light/directionLight.h"

class ShadowMapShader :public Shader {
public:
	ShadowMapShader(const std::vector<std::string> paths, std::shared_ptr<DirectionLight> light);
	void setCommonUniforms() const override;
	void setMeshUniforms(std::shared_ptr<Mesh> mesh) const override;
	void bind() const override;
	void unbind() const override;
private:
	std::shared_ptr<DirectionLight> light;
};