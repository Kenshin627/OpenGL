#pragma once
#include <glm/glm.hpp>

class Light
{
public:
	Light(const glm::vec3& c = glm::vec3(1.0f)) :color(c) {}
	const glm::vec3& getColor() const
	{
		return color;
	}
	virtual ~Light() {}
private:
	glm::vec3 color;
};