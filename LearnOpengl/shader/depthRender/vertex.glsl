#version 330 core

layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aCoords;

uniform mat4 modelViewProjection;

void main()
{
	gl_Position = modelViewProjection * vec4(aPosition, 1.0);
}