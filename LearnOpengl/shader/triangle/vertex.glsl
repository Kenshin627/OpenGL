#version 330 core
layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec2 aCoord;

uniform mat4 modelViewProjection;

out vec2 vCoord;

void main()
{
	vCoord = aCoord;
	gl_Position = modelViewProjection * vec4(aPosition, 1.0);
}