#version 330 core

layout(triangles) in;
layout(line_strip, max_vertices = 6) out;

in VS_OUT 
{
	vec3 vNormal;
} gs_in[];

uniform mat4 projection;
uniform float magnitude;

void GenerateLine(int index)
{
	gl_Position = projection * gl_in[index].gl_Position;
	EmitVertex();
	gl_Position = projection * ( gl_in[index].gl_Position + vec4(gs_in[index].vNormal, 0.0) * magnitude);
	EmitVertex();
	EndPrimitive();
}

void main()
{
	GenerateLine(0);
	GenerateLine(1);
	GenerateLine(2);
}