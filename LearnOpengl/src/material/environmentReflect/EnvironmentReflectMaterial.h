#pragma once
#include "../Material.h"

class Mesh;

class EnvironmentReflectMaterial :public Material
{
public:
	EnvironmentReflectMaterial(const X_Renderer& renderer);
	void setUniforms(const Mesh& mesh) override;
};