#pragma once
#include "../Material.h"

class Mesh;

class EnvironmentRefractMaterial :public Material
{
public:
	EnvironmentRefractMaterial(const X_Renderer& renderer, float refractIndex);
	void setUniforms(const Mesh& mesh) override;
private:
	float refractIndex;
};