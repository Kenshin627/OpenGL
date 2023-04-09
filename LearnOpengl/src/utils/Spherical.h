#pragma once
class Spherical
{
public:
	Spherical(float radius = 1.0f, float phi = 0.0f, float theta = 0.0f);
	Spherical& set(float radius, float phi, float theta);
	Spherical& copy(const Spherical& rhs);
	Spherical& makeSafe();
	Spherical& setFromVector3(const glm::vec3& v);
	Spherical& setFromCartesianCoords(float x, float y, float z);
	void setFromSpherical(glm::vec3& v) const;
public:
	float radius;
	float phi;
	float theta;
};