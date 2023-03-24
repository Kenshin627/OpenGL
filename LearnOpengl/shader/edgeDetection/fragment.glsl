#version 330 core

uniform sampler2D pipelineTexture;

in vec2 vCoords;

out vec4 fragColor;

const float offset = 1.0 / 300.0;

void main()
{
	vec2 coordsOffset[9] = vec2[](
		vec2(-offset, offset),  vec2(0, offset),  vec2(offset, offset),
		vec2(-offset, 0),		vec2(0, 0),		  vec2(offset, 0),
		vec2(-offset, -offset), vec2(0, -offset), vec2(offset, -offset)
	);

	float kernels[9] = float[](
		1,  1, 1,
		1, -8, 1,
		1,  1, 1
	);

	vec3 col = vec3(0, 0, 0);

	for(int i = 0; i < 9; i++)
	{
		col += vec3(texture(pipelineTexture, vCoords + coordsOffset[i])) * kernels[i];	
	}

	fragColor = vec4(col, 1.0);
}