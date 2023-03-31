#version 330 core

out vec4 outColor;
uniform float near;
uniform float far;
uniform vec3 nearColor;
uniform vec3 farColor;

float LinearizeDepth(float depth) 
{
    float z = depth * 2.0 - 1.0; // back to NDC 
    return (2.0 * near * far) / (far + near - z * (far - near));    
}

void main()
{
    float depth = LinearizeDepth(gl_FragCoord.z) / far;
	outColor = vec4(mix(nearColor, farColor, depth), 1.0);
}