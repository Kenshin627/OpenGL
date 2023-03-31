#include "pointLight.h"

PointLight::PointLight(const glm::vec3& p, const glm::vec3& c = glm::vec3(1.0), float r = 13):
	Light(c), 
	position(p), 
	attenuationMap({
		{7,	    AttenuationCofficient{ 1.0f, 0.7f,   1.8f      } },
		{13,	AttenuationCofficient{ 1.0f, 0.35f,  0.44f     } },
		{20,	AttenuationCofficient{ 1.0f, 0.22f,  0.20f     } },
		{32,	AttenuationCofficient{ 1.0f, 0.14f,  0.07f     } },
		{50,	AttenuationCofficient{ 1.0f, 0.09f,  0.032f    } },
		{65,	AttenuationCofficient{ 1.0f, 0.07f,  0.017f    } },
		{100,	AttenuationCofficient{ 1.0f, 0.045f, 0.0075f   } },
		{160,	AttenuationCofficient{ 1.0f, 0.027f, 0.0028f   } },
		{200,	AttenuationCofficient{ 1.0f, 0.022f, 0.0019f   } },
		{325,	AttenuationCofficient{ 1.0f, 0.014f, 0.0007f   } },
		{600,	AttenuationCofficient{ 1.0f, 0.007f, 0.0002f   } },
		{3250,	AttenuationCofficient{ 1.0f, 0.001f, 0.000007f } },
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