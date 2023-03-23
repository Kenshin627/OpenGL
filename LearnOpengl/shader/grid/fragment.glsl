#version 330 core

uniform vec2 viewport;

out vec4 outColor;

float grid_intensity = 0.7;

// Thick lines 
float grid(vec2 fragCoord, float space, float gridWidth)
{
    vec2 p  = fragCoord - vec2(.5);
    vec2 size = vec2(gridWidth);    
    vec2 a1 = mod(p - size, space);
    vec2 a2 = mod(p + size, space);
    vec2 a = a2 - a1;       
    float g = min(a.x, a.y);
    return clamp(g, 0., 1.0);
}

void main()
{
    // Pixel color
    vec3 col = vec3(.5,.5,.5);
    vec2 p = gl_FragCoord.xy;
	vec2 c = viewport.xy / 2.0;
    col *= (1.0 - length(c - p)/viewport.x*0.7);
    col *= clamp(grid(gl_FragCoord.xy, 10., 0.5) *  grid(gl_FragCoord.xy, 50., 1.), grid_intensity, 1.0);
    outColor = vec4(1.0, 0.0, 0.0, 1.0);
}