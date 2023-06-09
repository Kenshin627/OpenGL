#pragma once
#include <string>
#include "../../Mesh.h"


class Sphere :public Mesh
{
public:
	Sphere(const std::string& name, float radius = 7.0f, float widthSegments = 64, float heightSegments = 64, float phiStart = 0.0f, float phiLength = glm::two_pi<float>(), float thetaStart = 0.0f, float thetaLength = glm::pi<float>(), float uScale = 8.0f, float vScale = 4.0f, std::shared_ptr<Material> mat = nullptr);
private:
	void ConstructorVertex();
private:
	float radius;
	float widthSegments;
	float heightSegments;
	float thetaStart;
	float thetaLength;
	float phiStart;
	float phiLength;
	float uScale;
	float vScale;
};