#include "kspch.h"
#include "camera.h"
#include "../application/vendor/imGui/imgui.h"
#include "../utils/Quaternion.h"

Camera::Camera(const glm::vec3& position, const glm::vec3& target, const glm::vec3& up, float aspectRatio, float minZ, float maxZ, float fov, float moveSpeed, float sensitivity)
	:position(position), 
	direction(glm::normalize(target - position)),
	up(up),
	right(glm::vec3(1, 0, 0)), 
	target(target),
	aspectRatio(aspectRatio), 
	minZ(minZ), 
	maxZ(maxZ), 
	fov(fov), 
	sensitivity(sensitivity),
	moveSpeed(moveSpeed), 
	scale(1.0f),
	zoomSpeed(0.6f),
	zoomScale(std::pow(0.95f, zoomSpeed)),
	view(glm::identity<glm::mat4x4>()), 
	proj(glm::identity<glm::mat4x4>()),
	enableDamping(true),
	dampingFactor(0.1f),
	minRadius(0.0f),
	maxRadius(1000.0f)
{
	updateAxis();
	perspective();
	keyConfig = {
		{ ImGuiKey::ImGuiKey_W, MoveDirection::Forward },
		{ ImGuiKey::ImGuiKey_S, MoveDirection::Back },
		{ ImGuiKey::ImGuiKey_A,	MoveDirection::Left },
		{ ImGuiKey::ImGuiKey_D, MoveDirection::Right }
	};
}
	
Camera::~Camera() 
{

}

void Camera::lookAt()
{
	view = glm::lookAt(position, target, up);
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
	direction = glm::normalize(direction);
	right = glm::normalize(glm::cross(direction, up));
	up = glm::normalize(glm::cross(right, direction));
	lookAt();
}

void Camera::setRatio(float ratio)
{
	aspectRatio = ratio;
	perspective();
}

void Camera::setFov(float v)
{
	fov = v;
	perspective();
}

void Camera::orbitControl(float offsetX, float offsetY, float mouseWheel, const glm::vec2& viewport)
{
	if (mouseWheel < 0.0f)
	{
		scale /= zoomScale;
	}
	else if(mouseWheel > 0.0f) {
		scale *= zoomScale;
	}
	sphericalDelta.theta -= glm::two_pi<float>() * offsetX / static_cast<float>(viewport.y) * sensitivity;
	sphericalDelta.phi   -= glm::two_pi<float>() * offsetY / static_cast<float>(viewport.y) * sensitivity;
	glm::vec3 offset(0, 0, 0);

	const Quaternion quat = Quaternion().setFromUnitVectors(up, glm::vec3(0, 1, 0));
	const Quaternion quaInverse = Quaternion().copy(quat).invert();

	offset = position - target;
	quat.applyToVec(offset);
	spherical.setFromVector3(offset);

	if (enableDamping)
	{
		spherical.theta += sphericalDelta.theta * dampingFactor;
		spherical.phi += sphericalDelta.phi * dampingFactor;
	}
	else {
		spherical.theta += sphericalDelta.theta;
		spherical.phi += sphericalDelta.phi;
	}
	spherical.theta = glm::max<float>(minAzimuthAngle, glm::min<float>(maxAzimuthAngle, spherical.theta));
	spherical.phi = glm::max<float>(minPolarAngle, glm::min<float>(maxPolarAngle, spherical.phi));
	spherical.makeSafe();
	spherical.radius *= scale;
	spherical.radius = glm::max<float>(minRadius, glm::min<float>(spherical.radius, maxRadius));
	spherical.setFromSpherical(offset);
	quaInverse.applyToVec(offset);
	position = target + offset;
	direction = glm::normalize(target - position);
	lookAt();
	if (enableDamping) {
		sphericalDelta.theta *= (1 - dampingFactor);
		sphericalDelta.phi   *= (1 - dampingFactor);
	}
	else {
		sphericalDelta.set(0, 0, 0);
	}
	scale = 1.0f;
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