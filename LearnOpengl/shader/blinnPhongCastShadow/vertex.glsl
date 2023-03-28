#version 330 core

layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aUv;

out vec3 vPos;
out vec3 vNormal;
out vec2 vUv;
out vec4 fragPosLightSpace;

uniform mat4 modelViewProjection;
uniform mat4 model;
uniform mat3 normalMatrix;
uniform mat4 lightPosSpace;

void main()
{
	vPos = vec3(model * vec4(aPosition, 1.0));
	vNormal =  normalMatrix * aNormal;
	vUv = aUv;
	fragPosLightSpace = lightPosSpace * model * vec4(aPosition, 1.0);
	gl_Position = modelViewProjection * vec4(aPosition, 1.0);
}