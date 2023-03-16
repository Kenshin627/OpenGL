#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <string>
#include <unordered_map>
#include <glfw3.h>

enum MoveDirection
{
	Forward,
	Back,
	Left,
	Right
};

class Camera
{
	friend std::ostream& operator<<(std::ostream&, const Camera&);
public:
	Camera(const glm::vec3& position, const glm::vec3& direction, const glm::vec3& up, float aspectRatio, float minZ, float maxZ, float fov, float moveSpeed);
	~Camera();
	void lookAt();
	void perspective();
	void setPosition(const glm::vec3& p)
	{
		position = p;
		lookAt();
	}

	void setTarget(const glm::vec3& t)
	{
		direction = t;
		lookAt();
	}

	void setUp(const glm::vec3& u)
	{
		up = u;
		lookAt();
	}

	void setFov(float f)
	{
		fov = f;
		perspective();
	}

	void setMoveSpeed(float s)
	{
		moveSpeed = s;
	}

	void move(MoveDirection direction, float deltaTime);

	const glm::mat4& viewMatrix() const { return view; };
	const glm::mat4& projMatrix() const { return proj; };
	const std::unordered_map<unsigned, MoveDirection>& getKeybordConfig() const { return keyConfig; };

private:
	glm::vec3 position;
	glm::vec3 direction;
	glm::vec3 up;
	glm::vec3 right;
	float aspectRatio;
	float minZ;
	float maxZ;
	float fov;
	float moveSpeed;

	glm::mat4x4 view;
	glm::mat4x4 proj;
	
	std::unordered_map<unsigned, MoveDirection> keyConfig;
};
