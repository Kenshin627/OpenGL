#version 330 core

const float PI = 3.14159265359;

struct PointLight
{
	vec3 color;
	vec3 position;
};

struct PBRMaterial
{
	//PBR Material
	vec3 albedo;
	float roughness;
	float metallic;
	float ao;
};

out vec4 fragColor;

in vec3 fragWorldPos;
in vec3 vNormal;
in vec2 vCoords;

uniform vec3 camPosition;
uniform PointLight pls[4];
uniform PBRMaterial material;

float DistributionGGX(vec3 N, vec3 H, float roughness);
float GeometrySchlickGGX(float NdotV, float roughness);
float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness);
vec3 fresnelSchlick(float cosTheta, vec3 F0);

void main()
{
	vec3 N = normalize(vNormal);
	vec3 V = normalize(camPosition - fragWorldPos);
	vec3 lo = vec3(0.0);

	vec3 F0 =  vec3(0.04);
	F0 = mix(F0, material.albedo, material.metallic);

	for(int i = 0; i < 4; i++)
	{
		vec3 L = normalize(pls[i].position - fragWorldPos);
		vec3 H = normalize(L + V);
		float d = length(pls[i].position - fragWorldPos);
		//(pls[i].kc + pls[i].kl * d + pls[i].kd * d * d)
		float attenuation = 1.0 / (d * d);
		vec3 radiance = pls[i].color * attenuation;

		//cook-torrance brdf
		float NDF = DistributionGGX(N, H, material.roughness);
		float G = GeometrySmith(N, V, L, material.roughness);
		vec3 F = fresnelSchlick(clamp(dot(H, V), 0.0, 1.0), F0);

		vec3 numerator = NDF * F * G;
		float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.0001;
		vec3 specBRDF = numerator / denominator;
		vec3 kS = F;
		vec3 kD = vec3(1.0) - kS;
		kD *= 1.0 - material.metallic;

		float NdotL = max(dot(N, L), 0.0);

		lo += (kD * material.albedo/PI + specBRDF) * radiance * NdotL;
	}

	vec3 ambient = vec3(0.03) * material.albedo * material.ao;

	vec3 color = ambient + lo;

	//HDR toneMapping
	color = color /(color + vec3(1.0));

	//Gamma correct
	color = pow(color, vec3(1.0 / 2.2));

	fragColor = vec4(color, 1.0);
}

float DistributionGGX(vec3 N, vec3 H, float roughness)
{
    float a = roughness*roughness;
    float a2 = a*a;
    float NdotH = max(dot(N, H), 0.0);
    float NdotH2 = NdotH*NdotH;

    float nom   = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;

    return nom / denom;
}

float GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = (roughness + 1.0);
    float k = (r*r) / 8.0;

    float nom   = NdotV;
    float denom = NdotV * (1.0 - k) + k;

    return nom / denom;
}

float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2 = GeometrySchlickGGX(NdotV, roughness);
    float ggx1 = GeometrySchlickGGX(NdotL, roughness);

    return ggx1 * ggx2;
}

vec3 fresnelSchlick(float cosTheta, vec3 F0)
{
	return F0 + (1.0 - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}