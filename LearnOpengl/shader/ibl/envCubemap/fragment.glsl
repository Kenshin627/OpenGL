#version 330 core

out vec4 fragColor;

in vec3 vPos;

uniform sampler2D equirectangularMap;

const vec2 invAtan = vec2(0.15915494309189535, 0.3183098861837907);


vec2 SampleSphericalMap(in vec3 dir)
{
	vec2 uv = vec2(atan(dir.z, dir.x), asin(clamp( dir.y, - 1.0, 1.0 )));
	uv *= invAtan;
	uv += 0.5;
	return uv;
}


void main()
{
	vec2 uv = SampleSphericalMap(normalize(vPos));
	vec3 color = texture(equirectangularMap, uv).rgb;
	fragColor = vec4(color, 1.0);
}