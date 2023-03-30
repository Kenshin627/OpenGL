#version 330 core

uniform sampler2D pipelineA;
uniform sampler2D pipelineB;

in vec2 vCoords;
out vec4 fragColor;

void main()
{
	fragColor = vec4((texture(pipelineA, vCoords).rgb + texture(pipelineB, vCoords).rgb), 1.0);
}