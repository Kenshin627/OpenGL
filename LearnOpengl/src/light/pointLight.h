#pragma once
#include <unordered_map>
#include "baseLight.h"

struct AttenuationCofficient
{
	float kc;//常数项
	float kl;//一次项
	float kd;//二次项
};

class PointLight :public Light {
public:
	PointLight(const glm::vec3& p, const glm::vec3& c, float r);
	~PointLight();
	AttenuationCofficient getAttenuation() const;
	const glm::vec3& getPosition() const { return position; };
private:
	glm::vec3 position;
	std::unordered_map<float, AttenuationCofficient> attenuationMap;
	float radiationDistance;
};