#version 330 core

layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aUv;
layout (location = 3) in vec3 aTangent;
layout (location = 4) in vec3 aBitanget;

struct DirectionLight
{
	vec3 color;
	vec3 direction;
};

out vec3 vPositionTangentSpace;
out vec3 vCamPosTangentSpace;
out vec3 vLightDirectTangentSpace;

out vec2 vUv;


uniform mat4 modelViewProjection;
uniform mat4 model;
//uniform mat3 normalMatrix;

uniform DirectionLight directionLight;

uniform vec3 cameraPosition;

void main()
{
	vec3 T = normalize(vec3(model * vec4(aTangent, 0.0)));	
	vec3 N = normalize(vec3(model * vec4(aNormal, 0.0)));
	vec3 B = normalize(vec3(model * vec4(aBitanget, 0.0)));
//	T = normalize(T - dot(T, N) * N);
//	vec3 B = cross(N, T);

	vec3 calcN = cross(T, B);
	float normalsAligined = dot(calcN, N);
	if(normalsAligined < 0)
	{
		T = -T;
	}
	mat3 TBN = mat3(T, B, N);
	TBN = transpose(TBN);

	vPositionTangentSpace = TBN * aPosition;
	vCamPosTangentSpace = TBN * cameraPosition;
	vLightDirectTangentSpace = TBN * directionLight.direction;
	vUv = aUv;
	gl_Position = modelViewProjection * vec4(aPosition, 1.0);
}