#version 330 core

in vec3 vColor;
in vec2 vCoord;

uniform sampler2D sampler1;
uniform sampler2D sampler2;

out vec4 outColor;

void main()
{
	vec4 cTexture = texture(sampler1, vCoord);
	vec4 cTexture1 = texture(sampler2, vCoord);
	outColor = mix(cTexture, cTexture1, .5);	
}