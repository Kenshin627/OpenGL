#include "kspch.h"
#include "Spherical.h"

//theta->Ë®Æ½½Ç phi->´¹Ö±½Ç
Spherical::Spherical(float radius, float phi, float theta) :
	radius(radius),
	phi(phi),
	theta(theta) { }

Spherical& Spherical::set(float radius, float phi, float theta) 
{
    this->radius = radius;
    this->phi = phi;
    this->theta = theta;
    return *this;
}

Spherical& Spherical::copy(const Spherical& other) 
{
    this->radius = other.radius;
    this->phi = other.phi;
    this->theta = other.theta;
    return *this;
}

Spherical& Spherical::makeSafe() 
{
    const float EPS = 0.000001f;
    this->phi = glm::max<float>(EPS, glm::min<float>(glm::pi<float>() - EPS, this->phi));
    return *this;
}

Spherical& Spherical::setFromVector3(const glm::vec3& v) 
{
    return this->setFromCartesianCoords(v.x, v.y, v.z);
}

Spherical& Spherical::setFromCartesianCoords(float x, float y, float z) 
{
    this->radius = std::sqrt(x * x + y * y + z * z);
    if (this->radius == 0) {
        this->theta = 0;
        this->phi = 0;
    }
    else {

        this->theta = std::atan2(x, z);
        this->phi = std::acosf(glm::clamp<float>(y / this->radius, -1.f, 1.f));
    }
    return *this;
}

void Spherical::setFromSpherical(glm::vec3& v) const
{
    const auto sinPhiRadius = std::sin(phi) * radius;
    v.x = sinPhiRadius * std::sin(theta);
    v.y = std::cos(phi) * radius;
    v.z = sinPhiRadius * std::cos(theta);
}