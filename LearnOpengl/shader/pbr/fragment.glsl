#version 330 core

const float PI = 3.14159265359;

struct PointLight
{
	vec3 color;
	vec3 position;
};

struct PBRMaterial
{
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

//IBL
uniform samplerCube irradianceMap;
uniform samplerCube prefilterMap;
uniform sampler2D brdfLUT;

float DistributionGGX(vec3 N, vec3 H, float roughness);
float GeometrySchlickGGX(float NdotV, float roughness);
float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness);
vec3 fresnelSchlick(float cosTheta, vec3 F0);
vec3 fresnelSchlickRoughness(float cosTheta, vec3 F0, float roughness);

void main()
{
	vec3 N = normalize(vNormal);
	vec3 V = normalize(camPosition - fragWorldPos);
	vec3 R = reflect(-V, N);

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

	//vec3 ambient = vec3(0.03) * material.albedo * material.ao;
	vec3 F = fresnelSchlickRoughness(max(dot(N, V), 0.0), F0, material.roughness);
	vec3 kS = F;
	vec3 kD = 1.0 - kS;
	kD *= 1.0 - material.metallic;

	vec3 irradiance = texture(irradianceMap, N).rgb;
	vec3 diffuse = irradiance * material.albedo;

	//sample both the pre-filter map and the BRDF lut and combine them together as per the Split-Sum approximation to get the IBL specular part.
	const float MAX_REFLECTION_LOD = 4.0;
	vec3 prefilteredColor = textureLod(prefilterMap, R, material.roughness * MAX_REFLECTION_LOD).rgb;
	vec2 brdf = texture(brdfLUT, vec2(max(dot(N, V), 0.0), material.roughness)).rg;
	vec3 specular = prefilteredColor * (F * brdf.x + brdf.y);
	vec3 ambient = (kD * diffuse + specular) * material.ao;

	vec3 color = ambient + lo;

	//HDR toneMapping
//	color = color /(color + vec3(1.0));
//	float exposure = 1.5;
//	color = vec3(1.0) - exp(-color * exposure);

// optimized filmic operator by Jim Hejl and Richard Burgess-Dawson
	color *= 0.6;
	color = max( vec3( 0.0 ), color - 0.004 );
	color = pow( ( color * ( 6.2 * color + 0.5 ) ) / ( color * ( 6.2 * color + 1.7 ) + 0.06 ), vec3( 2.2 ) );

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

vec3 fresnelSchlickRoughness(float cosTheta, vec3 F0, float roughness)
{
    return F0 + (max(vec3(1.0 - roughness), F0) - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
} 
