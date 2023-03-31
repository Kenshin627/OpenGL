#include "../../mesh/Mesh.h"
#include "BlinnPhongMaterial.h"

BlinnPhongMaterial::BlinnPhongMaterial(std::vector<std::shared_ptr<Texture>> ambientTexture, std::vector<std::shared_ptr<Texture>> diffuseTexture, std::vector<std::shared_ptr<Texture>> specularTexture, std::vector<std::shared_ptr<Texture>> normalMap, float shininess, std::shared_ptr<X_Renderer> renderer):
	Material(ShaderType::BlinnPhong, renderer),
	ambientTexture(ambientTexture),
	diffuseTexture(diffuseTexture),
	specularTexture(specularTexture),
	shininess(shininess) {}

void BlinnPhongMaterial::setUniforms(const Mesh& mesh)
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
	program->setMatrix44("model", modelMatrix);
	//program->setMatrix33("normalMatrix", normalMatrix);
	if (!ambientTexture.empty())
	{
		ambientTexture[0]->bind(0);
		program->setInt("material.ambientTexture", 0);
	}
	
	if (!diffuseTexture.empty())
	{
		diffuseTexture[0]->bind(1);
		program->setInt("material.diffuseTexture", 1);
	}

	if (!specularTexture.empty())
	{
		specularTexture[0]->bind(2);
		program->setInt("material.specularTexture", 2);
	}

	if (!normalMap.empty())
	{
		normalMap[0]->bind(3);
		program->setInt("material.normalMap", 3);
	}

	program->setFloat("material.shininess", shininess);
}
