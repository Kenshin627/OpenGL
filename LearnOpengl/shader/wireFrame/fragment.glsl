#version 330 core

out vec4 outColor;
uniform vec3 wireFrameColor;

void main()
{
	outColor = vec4(wireFrameColor, 1.0);
}