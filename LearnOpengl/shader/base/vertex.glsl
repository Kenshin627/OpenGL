#version 330 core

layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aUv;

out vec2 vUv;
out vec3 vNormal;

uniform mat4 modelViewProjection;
uniform mat3 normalMatrix;

void main()
{
	vUv = aUv;
	vNormal = normalMatrix * aNormal;
	gl_Position = modelViewProjection * vec4(aPosition, 1.0);
}