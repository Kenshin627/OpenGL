
#version 330 core

layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aCoords;

uniform mat4 modelViewProjection;
uniform mat4 model;
uniform mat3 modelInverseTranspose;

out vec3 vPos;
out vec3 vNormal;

void main()
{
	vPos = vec3(model * vec4(aPosition, 1.0));
	vNormal = modelInverseTranspose * aNormal;
	gl_Position = modelViewProjection * vec4(aPosition, 1.0);
}