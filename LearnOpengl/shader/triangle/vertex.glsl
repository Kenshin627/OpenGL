#version 330 core
layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aCoord;

out vec3 vColor;
out vec2 vCoord;

void main()
{
	vColor = aColor;
	vCoord = aCoord;
	gl_Position =vec4(aPosition, 1.0);
}