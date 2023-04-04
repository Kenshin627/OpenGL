#version 330 core

uniform samplerCube skyBox;

in vec3 vCoords;
out vec4 fragColor;

void main()
{
	vec3 envColor = texture(skyBox, vCoords).rgb;
	envColor = envColor / (envColor + vec3(1.0));
	envColor = pow(envColor, vec3(1.0 / 2.2));
	fragColor = vec4(envColor, 1.0);
}