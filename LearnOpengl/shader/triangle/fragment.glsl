#version 330 core

in vec3 vColor;
in vec2 vCoord;

uniform vec4 uColor;
uniform sampler2D sampler;

out vec4 outColor;

void main()
{
	vec4 cTexture = texture(sampler, vCoord);
	outColor = mix(mix(vec4(vColor, 1.0), uColor, 0.5),cTexture, 0.5);
}