#version 330 core

layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aCoords;

out vec3 vNormal;
out vec4 fragPosLightSpace;

uniform mat4 modelViewProjection;
uniform mat3 normalMatrix;
uniform mat4 lightPosSpace;
uniform mat4 model;
void main()
{
	vNormal = normalMatrix * aNormal;
	fragPosLightSpace = lightPosSpace * model * vec4(aPosition, 1.0);
	gl_Position = modelViewProjection * vec4(aPosition, 1.0);
}