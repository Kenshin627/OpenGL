#include "camera.h"
#include "../application/vendor/imGui/imgui.h"

glm::mat4x4 rotationAlignTo(const glm::vec3& from, const glm::vec3& to)
{
	auto v = glm::cross(to, from);
	auto c = glm::dot(to, from);
	auto k = 1 / (1 + c);

	auto m = glm::mat4x4();
	m[0][0] = v.x * v.x * k + c;   m[0][1] = v.y * v.x * k - v.z;  m[0][2] = v.z * v.x * k + v.y;  m[0][3] = 0;
	m[1][0] = v.x * v.y * k + v.z; m[1][1] = v.y * v.y * k + c;    m[1][2] = v.z * v.y * k - v.x;  m[1][3] = 0;
	m[2][0] = v.x * v.z * k - v.y; m[2][1] = v.y * v.z * k + v.x;  m[2][2] = v.z * v.z * k + c;    m[2][3] = 0;
	m[3][0] = 0;                   m[3][1] = 0;                    m[3][2] = 0;                    m[3][3] = 1;

	return m;
}

void multiplyMMM(const glm::mat4x4& m, glm::vec3& vec)
{
	auto rx = vec.x * m[0][0] + vec.y * m[1][0] + vec.z * m[2][0] + m[3][0];
	auto ry = vec.x * m[0][1] + vec.y * m[1][1] + vec.z * m[2][1] + m[3][1];
	auto rz = vec.x * m[0][2] + vec.y * m[1][2] + vec.z * m[2][2] + m[3][2];
	float rw = 1.0f / (vec.x * m[0][3] + vec.y * m[1][3] + vec.z * m[2][3] + m[3][3]);


	vec.x = rx * rw;
	vec.y = ry * rw;
	vec.z = rz * rw;
}

Camera::Camera(const glm::vec3& position, const glm::vec3& target, const glm::vec3& up, float aspectRatio, float minZ, float maxZ, float fov, float moveSpeed, float sensitivity)
	:position(position), 
	direction(target - position), 
	up(up),
	right(glm::vec3(1, 0, 0)), 
	target(target),
	aspectRatio(aspectRatio), 
	minZ(minZ), 
	maxZ(maxZ), 
	fov(fov), 
	sensitivity(sensitivity),
	moveSpeed(moveSpeed), 
	alpha(0),
	beta(0),
	radius(0),
	uptoYMatrix(glm::identity<glm::mat4x4>()),
	ytoUpMatrix(glm::identity<glm::mat4x4>()),
	view(glm::identity<glm::mat4x4>()), 
	proj(glm::identity<glm::mat4x4>())
{
	updateAxis();
	buildAnglesAndRadius();
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

void Camera::buildAnglesAndRadius()
{
	//计算初始的alpha(水平角) beta(垂直角)
	glm::vec3 computationVec = position - target;

	if (up.x != 0.0 || up.y != 1.0f || up.z != 0.0)
	{
		multiplyMMM(uptoYMatrix, computationVec);
	}
	radius = glm::distance(position, target);
	if (radius == 0.0f)
	{
		radius = 0.0001f;
	}

	float previousAlpha = alpha;
	if (computationVec.x == 0.0f && computationVec.z == 0.0f)
	{
		alpha = glm::half_pi<float>();
	}
	else {
		alpha = acos(computationVec.x / sqrtf(pow(computationVec.x, 2) + pow(computationVec.z, 2)));
	}

	if (computationVec.z < 0)
	{
		alpha = glm::two_pi<float>() - alpha;
	}

	float alphaCorrectionTurns = roundf((previousAlpha - alpha) / (glm::two_pi<float>()));

	alpha += alphaCorrectionTurns * glm::two_pi<float>();
	beta = acos(computationVec.y / radius);

	//CHECK
	if (beta < 0.01f)
	{
		beta = 0.01f;
	}
	if (beta > glm::half_pi<float>())
	{
		beta = glm::half_pi<float>();
	}
}

void Camera::updateAxis()
{
	direction = glm::normalize(direction);
	right = glm::normalize(glm::cross(direction, up));
	up = glm::normalize(glm::cross(right, direction));

	ytoUpMatrix = rotationAlignTo(glm::vec3(0.0f, 1.0f, 0.0f), up);
	uptoYMatrix = rotationAlignTo(up, glm::vec3(0.0f, 1.0f, 0.0f));

	lookAt();
}

void Camera::pitchYaw(float xoffset, float yoffset)
{
	alpha += xoffset * sensitivity;
	beta  += yoffset * sensitivity;

	float x = cos(alpha) * sin(beta) * radius;
	float y = cos(beta) * radius;
	float z = sin(alpha) * sin(beta) * radius;
	glm::vec3 dir = glm::vec3(x, y, z);
	if (up.x != 0.0f || up.y != 1.0f || up.z != 0.0f)
	{
		multiplyMMM(ytoUpMatrix, dir);
	}
	
	position = target + dir;
	direction = glm::normalize(target - position);
	updateAxis();
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