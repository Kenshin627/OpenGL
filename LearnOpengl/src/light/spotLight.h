#pragma once
#include <glm/glm.hpp>
#include "baseLight.h"

class SpotLight :public Light {
public:
	SpotLight(const glm::vec3& p, const glm::vec3& d, const glm::vec3& c, float innerCutOff, float outterCutOff) :
		Light(c),
		position(p),
		direction(d),
		innerCutOff(innerCutOff),
		outterCutOff(outterCutOff)
	{
		
	}
	~SpotLight() {}
private:
	glm::vec3 position;
	glm::vec3 direction;
	float innerCutOff;
	float outterCutOff;
};