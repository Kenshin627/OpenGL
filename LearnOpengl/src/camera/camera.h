#pragma once
#include <GLFW/glfw3.h>
#include "../utils/Spherical.h"

enum MoveDirection
{
	Forward,
	Back,
	Left,
	Right
};

enum ImGuiKey;

class Camera
{
	friend std::ostream& operator<<(std::ostream&, const Camera&);
public:
	Camera(const glm::vec3& position, const glm::vec3& target, const glm::vec3& up, float aspectRatio, float minZ, float maxZ, float fov, float moveSpeed, float sensitivity);
	~Camera();
	void lookAt();
	void perspective();
	const glm::vec3& getPosition() const
	{
		return position;
	}

	const glm::vec3& getForward() const
	{
		return direction;
	}

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

	void setMoveSpeed(float s)
	{
		moveSpeed = s;
	}
	void move(MoveDirection direction, float deltaTime);
	void updateAxis();
	const glm::mat4& viewMatrix() const { return view; };
	const glm::mat4& projMatrix() const { return proj; };
	float getNear() const { return minZ; };
	float getFar() const { return maxZ; };
	const std::unordered_map<ImGuiKey, MoveDirection>& getKeybordConfig() const { return keyConfig; };
	void setRatio(float ratio);
	void setFov(float fov);
	void orbitControl(float xoffset, float yoffset, float mouseWheel, const glm::vec2& viewport);
private:
	glm::vec3 position;
	glm::vec3 direction;
	glm::vec3 up;
	glm::vec3 right;
	glm::vec3 target;
	float aspectRatio;
	float minZ;
	float maxZ;
	float fov;
	float sensitivity;
	float moveSpeed;
	float scale;
	float zoomSpeed;
	float zoomScale;
	glm::mat4x4 view;
	glm::mat4x4 proj;	
	std::unordered_map<ImGuiKey, MoveDirection> keyConfig;
	Spherical spherical;
	Spherical sphericalDelta;
	bool enableDamping;
	float dampingFactor;
	float minPolarAngle = 0.f;
	float maxPolarAngle = glm::pi<float>();
	float minAzimuthAngle = -std::numeric_limits<float>::infinity();// radians
	float maxAzimuthAngle = std::numeric_limits<float>::infinity(); // radians
	float minRadius;
	float maxRadius;
};
