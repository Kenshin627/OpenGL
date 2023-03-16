#include "camera.h"

Camera::Camera(const glm::vec3& position, const glm::vec3& direction, const glm::vec3& up, float aspectRatio, float minZ, float maxZ, float fov, float moveSpeed) :position(position), direction(direction), up(up),right(glm::vec3(1, 0, 0)), aspectRatio(aspectRatio), minZ(minZ), maxZ(maxZ), fov(fov), moveSpeed(moveSpeed), view(glm::identity<glm::mat4x4>()), proj(glm::identity<glm::mat4x4>())
{
	right = glm::cross(up, direction);
	lookAt();
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

void Camera::move(MoveDirection where)
{
	switch (where)
	{
		case Forward: position += direction * moveSpeed; break;
		case Back:    position -= direction * moveSpeed; break;
		case Left:	  position -= right * moveSpeed;     break;
		case Right:   position += right * moveSpeed;     break;
		default:                             break;		
	}
	lookAt();
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