#pragma once
#include "../Material.h"

class Mesh;

class BaseMaterial :public Material
{
public:
	BaseMaterial(const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3& specular, float shininess, const X_Renderer& renderer);
	void setUniforms(const Mesh& mesh) override;
private:
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
	float shininess;
};