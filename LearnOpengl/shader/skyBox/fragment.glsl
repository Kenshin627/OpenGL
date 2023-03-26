#version 330 core

uniform samplerCube skyBox;

in vec3 vCoords;
out vec4 fragColor;

void main()
{
	fragColor = texture(skyBox, vCoords);
}