#version 330 core

in vec3 vColor;
uniform vec4 uColor;
out vec4 outColor;

void main()
{
	outColor = mix(vec4(vColor, 1.0), uColor, 0.5);
}