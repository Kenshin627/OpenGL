#version 330 core

const float offset = 1.0 / 500.0;

in vec2 vCoords;
out vec4 fragColor;

uniform sampler2D pipeLineTexture;

void main()
{
	vec2 coordsOffsets[9] = vec2[](
		vec2(-offset, offset),  vec2(0.0f, offset),  vec2(offset, offset),
		vec2(-offset, 0.0f),       vec2(0.0f, 0.0f),       vec2(offset, 0.0f),
		vec2(-offset, -offset), vec2(0.0f, -offset), vec2(offset, -offset)
	);

	float kernels[9] = float[](
		-1, -1, -1,
		-1,  9, -1,
		-1, -1, -1
	);

	vec3 col = vec3(0, 0, 0);
	vec3 samplers[9];

	for(int i = 0; i < 9; i++)
	{
		samplers[i] = vec3(texture(pipeLineTexture, vCoords + coordsOffsets[i]));
	}


	for(int i = 0; i < 9; i++)
	{
		col += samplers[i] * kernels[i];
	}
	fragColor = vec4(col, 1.0);
}