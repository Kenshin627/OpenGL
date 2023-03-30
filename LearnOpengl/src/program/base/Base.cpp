#include "Base.h"

BaseShader::BaseShader(const std::vector<std::string> paths, std::shared_ptr<Camera> cam, std::shared_ptr<DirectionLight> light) :
	Shader(paths),
	camera(cam),
	light(light) {  }

void BaseShader::setCommonUniforms() const
{
	//uniform mat4 modelViewProjection;
	//uniform mat3 normalMatrix;
	// 
	//uniform DirectionLight directionLight;
	//uniform Material material;
	//uniform vec3 cameraPosition;

	//directionLight
	setVec3("directionLight.color", light->getColor());
	setVec3("directionLight.direction", light->getDirection());

	//camera
	setVec3("cameraPosition", camera->getPosition());
}

void BaseShader::setMeshUniforms(std::shared_ptr<Mesh> mesh) const
{
	//mvp
	glm::mat4x4 modelMatrix = mesh->getModelMatrix();
	glm::mat3x3 normalMatrix = glm::mat3x3(glm::transpose(glm::inverse(modelMatrix)));
	setMatrix44("modelViewProjection", camera->projMatrix() * camera->viewMatrix() * modelMatrix);
	setMatrix33("normalMatrix", normalMatrix);
}