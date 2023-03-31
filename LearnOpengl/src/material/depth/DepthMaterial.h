#pragma once
#include "../Material.h"

class Mesh;

class DepthMaterial :public Material
{
public:
	DepthMaterial(const X_Renderer& renderer, const glm::vec3& nearColor = glm::vec3(0.0, 0.0, 0.0), const glm::vec3& farColor = glm::vec3(1.0, 1.0, 1.0));
	void setUniforms(const Mesh& mesh) override;
private:
	glm::vec3 nearColor;
	glm::vec3 farColor;
};