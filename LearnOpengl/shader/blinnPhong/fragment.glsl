#version 330 core

struct DirectionLight
{
	vec3 color;
	vec3 direction;
};

struct PointLight
{
	vec3 color;
	vec3 position;
	float kc;
	float kl;
	float kd;
};

struct SpotLight
{
	vec3 color;
	vec3 position;
	vec3 direction;
	float innerCutOff;
	float outterCutOff;
};

struct Material
{
	vec3 diffuseColor;
	vec3 specularColor;
	vec3 ambientColor;
	float shininess;
};

in vec3 vPos;
in vec3 vNormal;
in vec2 vUv;

uniform DirectionLight directionLight;
uniform PointLight pointLight;
uniform Material material;
uniform vec3 cameraPosition;

out vec4 outColor;

vec3 calcBlinnphong(vec3 lightDirection);

void main()
{
	//direciton light
	vec3 lightDirection = normalize(directionLight.direction);
	vec3 directionColor = calcBlinnphong(lightDirection) * directionLight.color;

	//pointlight calc
	lightDirection = normalize(vPos - pointLight.position);
	float d = distance(vPos, pointLight.position);
	float attenuation = 1.0 / (pointLight.kc + pointLight.kl * d + pointLight.kd * d * d);
	vec3 pointColor = calcBlinnphong(lightDirection) * pointLight.color * attenuation;

	outColor = vec4(directionColor + pointColor, 1.0);
}

vec3 calcBlinnphong(vec3 lightDirection)
{
	vec3 normal = normalize(vNormal);
	vec3 eyeDirection = normalize(cameraPosition - vPos);
	vec3 lightDirectionReverse = -lightDirection;
	vec3 lightDirectionReflect = reflect(lightDirection, normal);
	//1. ambient
	vec3 ambient = material.ambientColor;

	//2. diffuse
	vec3 diffuse = max(dot(lightDirectionReverse, normal), 0.0) * material.diffuseColor;

	//3. specular
	vec3 specular = pow(max(dot(lightDirectionReflect, eyeDirection), 0.0), material.shininess) * material.specularColor;

	return ambient + diffuse + specular;
}