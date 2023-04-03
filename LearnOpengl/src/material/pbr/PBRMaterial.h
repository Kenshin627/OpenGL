#pragma once
#include "../Material.h"

class Mesh;

class PbrMaterial :public Material
{
public:
	PbrMaterial(const glm::vec3& albedo, float metallic, float roughness, float ao, const X_Renderer& renderer);
	void setUniforms(const Mesh& mesh) override;
private:
	glm::vec3 albedo;
	float metallic;
	float roughness;
	float ao;
};