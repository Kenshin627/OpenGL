#version 330 core

uniform sampler2D pipeLineTexture;

in vec2 vCoords;

out vec4 fragColor;

void main()
{
	fragColor = vec4(vec3(1.0 - texture(pipeLineTexture, vCoords)), 1.0);
}