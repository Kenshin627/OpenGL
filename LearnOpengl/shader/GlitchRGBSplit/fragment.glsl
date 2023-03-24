#version 330 core

uniform sampler2D pipeLineTexture;
uniform float intensity;
uniform float time;
uniform int direction;

in vec2 vCoords;

out vec4 outColor;

float randomNoise(float x, float y)
{
	return fract(sin(dot(vec2(x, y), vec2(12.9898, 78.233))) * 43758.5453);
}

void main()
{
	float splitAmount = intensity * randomNoise(time, 2.0);
	vec4 colorG = texture(pipeLineTexture, vCoords);
	if(direction == 0)
	{
		vec4 colorR = texture(pipeLineTexture, vec2(vCoords.x + splitAmount, vCoords.y));
		vec4 colorB = texture(pipeLineTexture, vec2(vCoords.x - splitAmount, vCoords.y));
		outColor = vec4(colorR.x, colorG.y, colorB.z, 1.0);
	}
	else if(direction == 1)
	{
		vec4 colorR = texture(pipeLineTexture, vec2(vCoords.x, vCoords.y + splitAmount));
		vec4 colorB = texture(pipeLineTexture, vec2(vCoords.x, vCoords.y - splitAmount));
		outColor = vec4(colorR.x, colorG.y, colorB.z, 1.0);
	}
	else {
		vec4 colorR = texture(pipeLineTexture, vec2(vCoords.x + splitAmount, vCoords.y + splitAmount));
		vec4 colorB = texture(pipeLineTexture, vec2(vCoords.x - splitAmount, vCoords.y - splitAmount));
		outColor = vec4(colorR.x, colorG.y, colorB.z, 1.0);
	}
}