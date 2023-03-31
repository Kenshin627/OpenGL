#pragma once
#include "../Material.h"

class Mesh;

class WireFrameMaterial :public Material
{
public:
	WireFrameMaterial(const X_Renderer& renderer, const glm::vec3& lineColor = glm::vec3(0.6, 0.6, 0.6));
	void setUniforms(const Mesh& mesh) override;
private:
	glm::vec3 lineColor;
};