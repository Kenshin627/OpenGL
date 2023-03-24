#version 330 core

layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aCoords;


out vec3 vPosition;
out vec3 vNormal;

uniform mat4 modelViewProjection;

void main()
{
	vPosition = aPosition;
	vNormal = aNormal;
	gl_Position = modelViewProjection * vec4(aPosition, 1.0);
}