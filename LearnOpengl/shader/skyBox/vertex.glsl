#version 330 core

layout (location = 0) in vec3 aPosition;

out vec3 vCoords;

uniform mat4 projection;
uniform mat4 skyBoxView;

void main()
{
	vCoords = aPosition;
	vec4 pos = projection * skyBoxView * vec4(aPosition, 1.0);
	gl_Position = pos.xyww;
}