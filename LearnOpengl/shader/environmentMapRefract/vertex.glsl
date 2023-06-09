#version 330 core

layout (location = 0) in vec3 aPostion;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aCoords;

uniform mat4 modelViewProjection;
uniform mat4 model;
uniform mat3 normalMatrix;

out vec3 vPos;
out vec3 vNormal;

void main()
{
	vPos = vec3(model * vec4(aPostion, 1.0));
	vNormal = normalMatrix * aNormal;
	gl_Position = modelViewProjection * vec4(aPostion, 1.0);
}