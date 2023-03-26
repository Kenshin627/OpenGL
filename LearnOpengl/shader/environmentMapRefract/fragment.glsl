#version 330 core

in vec3 vPos;
in vec3 vNormal;

uniform vec3 cameraPosition;
uniform samplerCube skyBox;
uniform float refractiveIndex;

out vec4 fragColor;

void main()
{
	float ratio = 1.0 / refractiveIndex;
	vec3 eyeDirection = normalize(vPos - cameraPosition);
	vec3 refractDirection = refract(eyeDirection, normalize(vNormal), ratio);
	fragColor = vec4(texture(skyBox, refractDirection).rgb, 1.0);
}