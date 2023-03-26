#version 330 core

in vec3 vPos;
in vec3 vNormal;

uniform vec3 cameraPosition;
uniform samplerCube skyBox;

out vec4 fragColor;

void main()
{
	vec3 eyeDirection = normalize(vPos - cameraPosition);
	vec3 normal = normalize(vNormal);
	vec3 reflectDir = reflect(eyeDirection, normal);
	fragColor = vec4(texture(skyBox, reflectDir).rgb, 1.0);
}