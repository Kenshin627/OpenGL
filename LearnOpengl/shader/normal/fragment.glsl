#version 330 core

out vec4 outColor;

in vec3 vNormal;

void main()
{
	outColor = vec4(vNormal, 1.0);
}