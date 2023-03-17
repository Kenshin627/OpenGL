#pragma once
#include "baseLight.h"

class DirectionLight :public Light {
public:
	DirectionLight(const glm::vec3& d = glm::vec3(0, -1, 0), const glm::vec3& c = glm::vec3(1.0)) :Light(c), direction(glm::normalize(d)) {}
	const glm::vec3& getDirection() const
	{
		return direction;
	}
private:
	glm::vec3 direction;
};