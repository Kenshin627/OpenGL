#version 330 core
layout(location = 0) in vec2 aPosition;
layout(location = 1) in vec2 aCoords;

out vec2 vCoords;

void main()
{
	vCoords = aCoords;
	gl_Position = vec4(aPosition, 0.0 , 1.0);
}