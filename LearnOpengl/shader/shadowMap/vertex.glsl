#version 330 core

layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec3 aNormal;
layout (location = 3) in vec2 aCoords;

uniform mat4 model;
uniform mat4 lightViewProjection;

void main()
{
	gl_Position = lightViewProjection * model * vec4(aPosition, 1.0);
}