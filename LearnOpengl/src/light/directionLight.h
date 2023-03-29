#pragma once
#include "baseLight.h"

class DirectionLight :public Light {
public:
	DirectionLight(const glm::vec3& d = glm::vec3(0, -1, 0), const glm::vec3& c = glm::vec3(1.0)) :Light(c), direction(glm::normalize(d))
	{
		calcLightSpaceMatrix();
	}
	glm::vec3& getDirection()
	{
		return direction;
	}

	void setDirection(const glm::vec3& direc)
	{
		if (direc != direction)
		{
			direction = glm::normalize(direc);
			calcLightSpaceMatrix();
		}
	}

	glm::vec3 getPostion() const 
	{
		return position;
	}
private:
	void calcLightSpaceMatrix()
	{
		auto center = glm::vec3(0.0f, 0.0f, 0.0f);
		position = center - direction - direction - direction - direction - direction - direction - direction - direction;
		float orthoSize = 20.0f;

		glm::mat4x4 lightProj = glm::ortho(-orthoSize, orthoSize, -orthoSize, orthoSize, 0.01f, 100.0f);
		glm::mat4x4 lightView = glm::lookAt(position, center, glm::vec3(0.0f, 1.0f, 0.0f));
		lightSpaceMatrix = lightProj * lightView;
	}
private:
	glm::vec3 direction;
	glm::vec3 position;
};