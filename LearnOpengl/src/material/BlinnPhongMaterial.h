#pragma once
#include <glm/glm.hpp>

class BlinnPhongMaterial
{
public:
	BlinnPhongMaterial(const glm::vec3& d, const glm::vec3& s, const glm::vec3& a, float shininess):
		diffuseColor(d), 
		specularColor(s), 
		ambientColor(a), 
		shininess(shininess)
	{
	}
	const glm::vec3& getDiffuseColor() const
	{
		return diffuseColor;
	}
	const glm::vec3& getSpecularColor() const
	{
		return specularColor;
	}
	const glm::vec3& getAmbientColor() const
	{
		return ambientColor;
	}
	float getShininess() const
	{
		return shininess;
	}
	~BlinnPhongMaterial() {}
private:
	glm::vec3 diffuseColor;
	glm::vec3 specularColor;
	glm::vec3 ambientColor;
	float shininess;
};