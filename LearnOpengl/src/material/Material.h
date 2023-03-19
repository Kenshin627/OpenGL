#pragma once
#include <glm/glm.hpp>
#include "../Texture/Texture.h"

class Material
{
public:
	Material():diffuse(glm::vec3()), specular(glm::vec3()) , ambient(glm::vec3()) , shininess(0.0f) {}
	glm::vec3 diffuse;
	glm::vec3 specular;
	glm::vec3 ambient;
	float shininess;
	std::vector<std::shared_ptr<Texture>> ambientTextures;
	std::vector<std::shared_ptr<Texture>> diffuseTextures;
	std::vector<std::shared_ptr<Texture>> specularTextures;
};