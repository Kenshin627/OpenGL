#pragma once
#include <string>
#include "../../Mesh.h"

class TorusKnot :public Mesh
{
public:
	TorusKnot(const std::string& name, float radius, float tube = 1.5f, unsigned tubelarSegments = 64.0f, unsigned radialSegments = 32.0f, float p = 2, float q = 3, std::shared_ptr<Material> mat = nullptr);
	~TorusKnot();
private:
	void ConstructorVertex();
	glm::vec3 calculatePositionOnCurve(float u, float p, float q, float radius) const;
private:
	float radius;
	float tube;
	unsigned tubelarSegments;
	unsigned radialSegments;
	float p;
	float q;
};