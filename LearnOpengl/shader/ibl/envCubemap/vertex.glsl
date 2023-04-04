#version 330 core

layout (location = 0) in vec3 aPosition;

uniform mat4 captureView;
uniform mat4 captureProjection;

out vec3 vPos;

void main()
{
	vPos = aPosition;
	gl_Position = captureProjection * captureView * vec4(vPos, 1.0);
}