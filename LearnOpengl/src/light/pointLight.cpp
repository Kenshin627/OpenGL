#include "pointLight.h"

PointLight::PointLight(const glm::vec3& p, const glm::vec3& c = glm::vec3(1.0), float r = 13):
	Light(c), 
	position(p), 
	attenuationMap({
		{7,	    AttenuationCofficient{ 1.0,	0.7,   1.8      } },
		{13,	AttenuationCofficient{ 1.0,	0.35,  0.44     } },
		{20,	AttenuationCofficient{ 1.0,	0.22,  0.20     } },
		{32,	AttenuationCofficient{ 1.0,	0.14,  0.07     } },
		{50,	AttenuationCofficient{ 1.0,	0.09,  0.032    } },
		{65,	AttenuationCofficient{ 1.0,	0.07,  0.017    } },
		{100,	AttenuationCofficient{ 1.0,	0.045, 0.0075   } },
		{160,	AttenuationCofficient{ 1.0,	0.027, 0.0028   } },
		{200,	AttenuationCofficient{ 1.0,	0.022, 0.0019   } },
		{325,	AttenuationCofficient{ 1.0,	0.014, 0.0007   } },
		{600,	AttenuationCofficient{ 1.0,	0.007, 0.0002   } },
		{3250,	AttenuationCofficient{ 1.0,	0.001, 0.000007 } },
	}),
	radiationDistance(r)
{

}

PointLight::~PointLight() {}

AttenuationCofficient PointLight::getAttenuation() const
{
	auto ret = attenuationMap.find(radiationDistance);
	if (ret != attenuationMap.cend())
	{
		return ret->second;
	}
	return AttenuationCofficient();
}