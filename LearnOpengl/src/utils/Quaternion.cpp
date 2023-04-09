#include "kspch.h"
#include "Quaternion.h"

Quaternion::Quaternion(float x, float y, float z, float w) :
    x(x), y(y), z(z), w(w) {}
float Quaternion::operator[](unsigned index) const
{
    switch (index) {
    case 0:
        return x;
    case 1:
        return y;
    case 2:
        return z;
    case 3:
        return w;
    default:
        return -1;
    }
}

Quaternion& Quaternion::copy(const Quaternion& qua)
{
    this->x = qua[0];
    this->y = qua[1];
    this->z = qua[2];
    this->w = qua[3];
    return *this;
}

Quaternion& Quaternion::setFromUnitVectors(const glm::vec3& vFrom, const glm::vec3& vTo)
{
    // assumes direction vectors vFrom and vTo are normalized
    const auto EPS = 0.000001f;
    auto r = glm::dot(vFrom, vTo) + 1;
    if (r < EPS) {
        r = 0;
        if (std::abs(vFrom.x) > std::abs(vFrom.z)) {
            this->x = -vFrom.y;
            this->y = vFrom.x;
            this->z = 0;
            this->w = r;
        }
        else {
            this->x = 0;
            this->y = -vFrom.z;
            this->z = vFrom.y;
            this->w = r;
        }
    }
    else {
        this->x = vFrom.y * vTo.z - vFrom.z * vTo.y;
        this->y = vFrom.z * vTo.x - vFrom.x * vTo.z;
        this->z = vFrom.x * vTo.y - vFrom.y * vTo.x;
        this->w = r;
    }
    return this->normalize();
}

Quaternion& Quaternion::invert()
{
    return this->conjugate();
}

Quaternion& Quaternion::conjugate()
{
    this->x *= -1;
    this->y *= -1;
    this->z *= -1;
    //this->onChangeCallback_();
    return *this;
}

Quaternion& Quaternion::normalize()
{
    auto l = length();
    if (l == 0) {
        this->x = 0;
        this->y = 0;
        this->z = 0;
        this->w = 1;
    }
    else {
        l = 1.0f / l;
        this->x = this->x * l;
        this->y = this->y * l;
        this->z = this->z * l;
        this->w = this->w * l;
    }
    //this->onChangeCallback_();
    return *this;
}

float Quaternion::length() const
{
    return std::sqrt(this->x * this->x + this->y * this->y + this->z * this->z + this->w * this->w);
}

void Quaternion::applyToVec(glm::vec3& v) const
{
    const auto x = v.x, y = v.y, z = v.z;
    const auto qx = this->x, qy = this->y, qz = this->z, qw = this->w;

    const auto ix = qw * x + qy * z - qz * y;
    const auto iy = qw * y + qz * x - qx * z;
    const auto iz = qw * z + qx * y - qy * x;
    const auto iw = -qx * x - qy * y - qz * z;

    v.x = ix * qw + iw * -qx + iy * -qz - iz * -qy;
    v.y = iy * qw + iw * -qy + iz * -qx - ix * -qz;
    v.z = iz * qw + iw * -qz + ix * -qy - iy * -qx;
}