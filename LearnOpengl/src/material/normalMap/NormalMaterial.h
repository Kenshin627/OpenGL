#pragma once
#include "../Material.h"

class Mesh;

class NormalMaterial :public Material
{
public:
	NormalMaterial(const X_Renderer& renderer);
	void setUniforms(const Mesh& mesh) override;
};