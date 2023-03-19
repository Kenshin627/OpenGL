#include "camera.h"

Camera::Camera(const glm::vec3& position, const glm::vec3& direction, const glm::vec3& up, float aspectRatio, float minZ, float maxZ, float fov, float moveSpeed, float sensitivity)
	:position(position), 
	direction(direction), 
	up(up),
	right(glm::vec3(1, 0, 0)), 
	aspectRatio(aspectRatio), 
	minZ(minZ), 
	maxZ(maxZ), 
	fov(fov), 
	sensitivity(sensitivity),
	moveSpeed(moveSpeed), 
	yaw(-90),
	pitch(0),
	view(glm::identity<glm::mat4x4>()), 
	proj(glm::identity<glm::mat4x4>())
{
	updateAxis();

	perspective();
	keyConfig = {
		{ GLFW_KEY_W, MoveDirection::Forward },
		{ GLFW_KEY_S, MoveDirection::Back },
		{ GLFW_KEY_A,	MoveDirection::Left },
		{ GLFW_KEY_D, MoveDirection::Right }
	};
}
	
Camera::~Camera() 
{

}

void Camera::lookAt()
{
	view = glm::lookAt(position, position + direction, up);
}

void Camera::perspective()
{
	proj = glm::perspective(fov, aspectRatio, minZ, maxZ);
}

void Camera::move(MoveDirection where, float deltaTime)
{
	switch (where)
	{
		case Forward: position += direction * moveSpeed * deltaTime; break;
		case Back:    position -= direction * moveSpeed * deltaTime; break;
		case Left:	  position -= right		* moveSpeed * deltaTime; break;
		case Right:   position += right		* moveSpeed * deltaTime; break;
		default:													 break;		
	}
	lookAt();
}

void Camera::updateAxis()
{
	glm::vec3 front;
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	direction = glm::normalize(front);
	right = glm::normalize(glm::cross(direction, up));
	up = glm::normalize(glm::cross(right, front));
	lookAt();
}

void Camera::pitchYaw(float xoffset, float yoffset)
{
	yaw += xoffset * sensitivity;
	pitch+= yoffset * sensitivity;
	
	updateAxis();
}

std::ostream& operator<<(std::ostream& out, const Camera& camera)
{
	std::cout << "[POSITION]:\t" << "(" << camera.position.x
		<< ","
		<< camera.position.y
		<< ","
		<< camera.position.z
		<< ")"
		<< std::endl;
	std::cout << "[TARGET]:\t" << "(" << camera.direction.x
		<< ","
		<< camera.direction.y
		<< ","
		<< camera.direction.z
		<< ")"
		<< std::endl;
	std::cout << "[FOV]:\t" << camera.fov << std::endl;
	return std::cout << "[ASPECTRATIO]:\t" << camera.aspectRatio << std::endl;
}