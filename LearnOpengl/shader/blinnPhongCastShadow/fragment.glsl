#version 330 core

struct DirectionLight
{
	vec3 color;
	vec3 direction;
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
in vec4 fragPosLightSpace;

uniform DirectionLight directionLight;
uniform Material material;
uniform vec3 cameraPosition;
uniform sampler2D shadowMap;

out vec4 outColor;

blinnPhongCoffient calcBlinnphong(vec3 lightDirection);

float calcShadow()
{
	vec3 fragCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
	fragCoords = fragCoords * 0.5 + 0.5;
	vec3 normal = normalize(vNormal);
	vec3 lightDirectionReverse = -normalize(directionLight.direction);
	float shadowBias = min(0.005, (1.0 - max(dot(lightDirectionReverse, normal), 0.0)) * 0.05);
	float closetDepth = texture(shadowMap, fragCoords.xy).r;
	return (fragCoords.z - shadowBias) > closetDepth? 1.0 : 0.0; 
}

void main()
{
	//Direciton light
	vec3 lightDirection = normalize(directionLight.direction);
	blinnPhongCoffient coffient = calcBlinnphong(lightDirection);
	float shadow = calcShadow();
	vec3 directionColor = (coffient.ambient + coffient.diffuse * (1.0 - shadow) + coffient.specular * (1.0 - shadow)) * directionLight.color;

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
