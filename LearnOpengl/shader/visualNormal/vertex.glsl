#version 330 core

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aCoords;

uniform mat4 modelView;
uniform mat3 inverseModelView;

out VS_OUT 
{
	vec3 vNormal;
} vs_out;

void main()
{
	vs_out.vNormal = normalize(vec3(vec4(inverseModelView * aNormal, 0.0)));
	gl_Position = modelView * vec4(aPosition, 1.0);
}