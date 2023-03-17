#pragma once
#include <glm/glm.hpp>
#include "pointLight.h"

class SpotLight :public PointLight {
public:
	SpotLight(const glm::vec3& p, const glm::vec3& d, const glm::vec3& c, float r, float innerCutOff, float outterCutOff) :
		PointLight(p, c, r),
		direction(glm::normalize(d)),
		innerCutOff(innerCutOff),
		outterCutOff(outterCutOff)
	{
		
	}
	~SpotLight() {}
	const glm::vec3& getDirection() const { return direction; };
	float getInnerCutOff() const { return innerCutOff; };
	float getOutterCutOff() const { return outterCutOff; };
private:
	glm::vec3 direction;
	float innerCutOff;
	float outterCutOff;
};