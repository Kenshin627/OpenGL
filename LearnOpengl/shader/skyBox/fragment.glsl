#version 330 core

uniform samplerCube skyBox;

in vec3 vCoords;
out vec4 fragColor;

void main()
{
// texture(skyBox, vCoords)
	fragColor = texture(skyBox, vCoords);
}