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
	float kc;
	float kl;
	float kd;
};

struct Material
{
	vec3 diffuseColor;
	vec3 specularColor;
	vec3 ambientColor;
	float shininess;
	sampler2D ambientTexture;
	sampler2D diffuseTexture;
	sampler2D specularTexture;
};

struct blinnPhongCoffient
{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

in vec3 vPos;
in vec3 vNormal;
in vec2 vUv;

uniform DirectionLight directionLight;
uniform PointLight pointLight;
uniform SpotLight spotLight;
uniform Material material;
uniform vec3 cameraPosition;

out vec4 outColor;

blinnPhongCoffient calcBlinnphong(vec3 lightDirection);

void main()
{
	//Direciton light
	vec3 lightDirection = normalize(directionLight.direction);
	blinnPhongCoffient coffient = calcBlinnphong(lightDirection);
	vec3 directionColor = (coffient.ambient + coffient.diffuse + coffient.specular) * directionLight.color;

	//Pointlight calc
	lightDirection = normalize(vPos - pointLight.position);
	float d = distance(vPos, pointLight.position);
	float attenuation = 1.0 / (pointLight.kc + pointLight.kl * d + pointLight.kd * d * d);
	coffient = calcBlinnphong(lightDirection);
	vec3 pointColor =  (coffient.ambient + coffient.diffuse + coffient.specular) * pointLight.color * attenuation;

	//SpotLight calc
	lightDirection = normalize(vPos - spotLight.position);
	d = distance(vPos, spotLight.position);
	attenuation = 1.0 / (spotLight.kc + spotLight.kl * d + spotLight.kd * d * d);
	coffient = calcBlinnphong(lightDirection);

	float theta = dot(lightDirection, normalize(spotLight.direction));
	float epsilon = (spotLight.innerCutOff - spotLight.outterCutOff);
	float intensity = clamp((theta - spotLight.outterCutOff) / epsilon, 0.0, 1.0);
	
	vec3 spotColor = (coffient.ambient + coffient.diffuse * intensity + coffient.specular*intensity) * spotLight.color * attenuation;

	outColor = vec4(directionColor, 1.0);
}

blinnPhongCoffient calcBlinnphong(vec3 lightDirection)
{
	vec3 normal = normalize(vNormal);
	vec3 eyeDirection = normalize(cameraPosition - vPos);
	vec3 lightDirectionReverse = -lightDirection;
	//1. ambient

	vec3 ambient = vec3(texture(material.ambientTexture, vUv));
	//vec3 ambient = material.ambientColor;

	//2. diffuse

	vec3 diffuseC = vec3(texture(material.diffuseTexture, vUv));
	vec3 diffuse = max(dot(lightDirectionReverse, normal), 0.0) * diffuseC;

	//3. specular
	vec3 specularC = vec3(texture(material.specularTexture, vUv));
	vec3 halfwayDir = normalize(eyeDirection + lightDirectionReverse);
	vec3 specular  = pow(max(dot(halfwayDir, normal), 0.0), material.shininess) * specularC;

	return blinnPhongCoffient(ambient, diffuse, specular);
}
