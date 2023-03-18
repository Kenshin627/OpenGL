#pragma once
#include <glm/glm.hpp>

class Material
{
public:
	Material():diffuse(glm::vec3()), specular(glm::vec3()) , ambient(glm::vec3()) , shininess(0.0f) {}
	glm::vec3 diffuse;
	glm::vec3 specular;
	glm::vec3 ambient;
	float shininess;
};