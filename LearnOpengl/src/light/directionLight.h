#pragma once
#include "baseLight.h"

class DirectionLight :public Light {
public:
	DirectionLight(const glm::vec3& d = glm::vec3(0, -1, 0), const glm::vec3& c = glm::vec3(1.0)) :Light(c), direction(glm::normalize(d))
	{
		auto center = glm::vec3(0.0f, 0.0f, 0.0f);
		auto eye = center - direction - direction;
		float orthoSize = 5.0f;

		glm::mat4x4 lightProj = glm::ortho(-orthoSize, orthoSize, -orthoSize, orthoSize, 0.01f, 100.0f);
		glm::mat4x4 lightView = glm::lookAt(eye, center, glm::vec3(0.0f, 1.0f, 0.0f));
		lightSpaceMatrix = lightProj * lightView;
	}
	const glm::vec3& getDirection() const
	{
		return direction;
	}
private:
	glm::vec3 direction;
};