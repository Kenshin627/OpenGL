#include "../../mesh/Mesh.h"
#include "PBR2Material.h"

Pbr2Material::Pbr2Material(std::shared_ptr<Texture> albedo, std::shared_ptr<Texture> metallic, std::shared_ptr<Texture> roughness, std::shared_ptr<Texture> normalMap, std::shared_ptr<Texture> ao, const X_Renderer& renderer) :Material(ShaderType::PBR2, renderer),
albedo(albedo),
metallic(metallic),
roughness(roughness),
normalMap(normalMap),
ao(ao) { }

void Pbr2Material::setUniforms(const Mesh& mesh)
{
	glm::mat4x4 modelMatrix = mesh.getModelMatrix();
	std::shared_ptr<Camera> camera = renderer.getCamera();
	program->setMatrix44("model", modelMatrix);
	glm::mat3x3 normalMatrix = glm::mat3x3(glm::transpose(glm::inverse(modelMatrix)));
	program->setMatrix33("normalMatrix", normalMatrix);
	program->setMatrix44("modelViewProjection", camera->projMatrix() * camera->viewMatrix() * modelMatrix);
	program->setVec3("camPosition", camera->getPosition());

	albedo->bind(0);
	program->setInt("material.albedo", 0);
	metallic->bind(1);
	program->setInt("material.metallic", 1);
	roughness->bind(2);
	program->setInt("material.roughness", 2);
	normalMap->bind(3);
	program->setInt("material.normalMap", 3);
	ao->bind(4);
	program->setInt("material.ao", 4);

	auto pointLights = renderer.getPointLights();
	unsigned pointLightSize = pointLights.size();

	for (unsigned i = 0; i < pointLightSize; i++)
	{
		program->setVec3("pls[" + std::to_string(i) + "].color", pointLights[i]->getColor());
		program->setVec3("pls[" + std::to_string(i) + "].position", pointLights[i]->getPosition());
		//program->setFloat("pls[" + std::to_string(i) + "].kc", pointLights[i]->getAttenuation().kc);
		//program->setFloat("pls[" + std::to_string(i) + "].kl", pointLights[i]->getAttenuation().kl);
		//program->setFloat("pls[" + std::to_string(i) + "].kd", pointLights[i]->getAttenuation().kd);
	}
}