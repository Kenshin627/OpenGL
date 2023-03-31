#pragma once
#include "../Material.h"

class Mesh;

class GridMaterial :public Material
{
public:
	GridMaterial(glm::vec3 mainColor, glm::vec3 lineColor, glm::vec3 gridOffset, glm::vec4 gridControl, std::shared_ptr<X_Renderer> renderer);
	void setUniforms(const Mesh& mesh) override;
private:
	glm::vec3 mainColor;
	glm::vec3 lineColor;
	glm::vec3 gridOffset;
	glm::vec4 gridControl;
};