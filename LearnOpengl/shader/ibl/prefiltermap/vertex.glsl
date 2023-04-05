#version 330 core
layout (location = 0) in vec3 aPosition;

out vec3 vPos;

uniform mat4 captureProjection;
uniform mat4 captureView;

void main()
{
    vPos = aPosition;  
    gl_Position =  captureProjection * captureView * vec4(aPosition, 1.0);
}