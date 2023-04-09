#pragma once

class Quaternion
{
public:
    explicit Quaternion(float x = 0, float y = 0, float z = 0, float w = 1);
    float operator[](unsigned index) const;
    Quaternion& copy(const Quaternion& qua);
    Quaternion& setFromUnitVectors(const glm::vec3& vFrom, const glm::vec3& vTo);
    Quaternion& invert();
    Quaternion& conjugate();
    Quaternion& normalize();
    float length() const;
    void applyToVec(glm::vec3& v) const;
private:
	float x;
	float y;
	float z;
	float w;
};