#version 330 core

uniform sampler2D pipeLineTexture;

in vec2 vCoords;

out vec4 fragColor;

void main()
{
	vec4 samplerColor = texture(pipeLineTexture, vCoords);
	float avgColor = 0.2126 * samplerColor.r + 0.7152 * samplerColor.g + 0.0722 * samplerColor.b;
	fragColor = vec4(vec3(avgColor), 1.0);
}