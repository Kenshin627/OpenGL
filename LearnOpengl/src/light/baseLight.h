#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Light
{
public:
	Light(const glm::vec3& c = glm::vec3(1.0f)) :color(c), lightSpaceMatrix(glm::identity<glm::mat4x4>()) {}
	const glm::vec3& getColor() const
	{
		return color;
	}
	virtual ~Light() {}
	const glm::mat4x4& getLightSpaceMatrix() const { return lightSpaceMatrix; };
protected:
	glm::vec3 color;
	glm::mat4x4 lightSpaceMatrix;
};