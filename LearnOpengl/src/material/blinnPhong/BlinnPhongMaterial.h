#pragma once
#include "../Material.h"
#include "../../Texture/Texture.h"

class Mesh;

class BlinnPhongMaterial :public Material
{
public:
	BlinnPhongMaterial(std::vector<std::shared_ptr<Texture>> ambientTexture, std::vector<std::shared_ptr<Texture>> diffuseTexture, std::vector<std::shared_ptr<Texture>> specularTexture, std::vector<std::shared_ptr<Texture>> normalMap, float shininess, std::shared_ptr<X_Renderer> renderer);
	void setUniforms(const Mesh& mesh) override;
public:
	std::vector<std::shared_ptr<Texture>> ambientTexture;
	std::vector<std::shared_ptr<Texture>> diffuseTexture;
	std::vector<std::shared_ptr<Texture>> specularTexture;
	std::vector<std::shared_ptr<Texture>> normalMap;
	float shininess;
};