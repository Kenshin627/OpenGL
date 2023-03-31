#include "../../mesh/Mesh.h"
#include "BaseMaterial.h"

BaseMaterial::BaseMaterial(const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3& specular, float shininess, std::shared_ptr<X_Renderer> renderer):
	Material(ShaderType::Base, renderer),
	ambient(ambient),
	diffuse(diffuse),
	specular(specular),
	shininess(shininess) {}

void BaseMaterial::setUniforms(const Mesh& mesh)
{
	//lights
	auto light = renderer->getLights()[0];
	program->setVec3("directionLight.color", light->getColor());
	program->setVec3("directionLight.direction", light->getDirection());

	//camera
	auto camera = renderer->getCamera();
	program->setVec3("cameraPosition", camera->getPosition());

	//mvp
	glm::mat4x4 modelMatrix = mesh.getModelMatrix();
	glm::mat3x3 normalMatrix = glm::mat3x3(glm::transpose(glm::inverse(modelMatrix)));
	program->setMatrix44("modelViewProjection", camera->projMatrix() * camera->viewMatrix() * modelMatrix);
	program->setMatrix33("normalMatrix", normalMatrix);
	program->setVec3("material.ambientColor", ambient);
	program->setVec3("material.diffuseColor", diffuse);
	program->setVec3("material.specularColor", specular);
	program->setFloat("material.shininess", shininess);
}
