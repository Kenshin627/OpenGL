#version 330 core

layout (location = 0) in vec3 aPosition;

uniform mat4 captureview;
uniform mat4 captureProjection;

out vec3 vPos;

void main()
{
	vPos = aPosition;
	gl_Position = captureProjection * captureview * vec4(aPosition, 1.0);
}