#pragma once
#include "../Material.h"

class Mesh;

class Pbr2Material :public Material
{
public:
	Pbr2Material(std::shared_ptr<Texture> albedo, std::shared_ptr<Texture> metallic, std::shared_ptr<Texture> roughness, std::shared_ptr<Texture> normalMap, std::shared_ptr<Texture> ao, const X_Renderer& renderer);
	void setUniforms(const Mesh& mesh) override;
private:
	std::shared_ptr<Texture> albedo;
	std::shared_ptr<Texture> metallic;
	std::shared_ptr<Texture> roughness;
	std::shared_ptr<Texture> normalMap;
	std::shared_ptr<Texture> ao;
};