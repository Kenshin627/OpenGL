#include <glad/gl.h>
#include "../../mesh/Mesh.h"
#include "PBRMaterial.h"

PbrMaterial::PbrMaterial(const glm::vec3& albedo, float metallic, float roughness, float ao, const X_Renderer& renderer) :Material(ShaderType::PBR, renderer),
albedo(albedo),
metallic(metallic),
roughness(roughness),
ao(ao) { }

void PbrMaterial::setUniforms(const Mesh& mesh)
{
	glm::mat4x4 modelMatrix = mesh.getModelMatrix();
	std::shared_ptr<Camera> camera = renderer.getCamera();
	program->setMatrix44("model", modelMatrix);
	glm::mat3x3 normalMatrix = glm::mat3x3(glm::transpose(glm::inverse(modelMatrix)));
	program->setMatrix33("normalMatrix", normalMatrix);
	program->setMatrix44("modelViewProjection", camera->projMatrix() * camera->viewMatrix() * modelMatrix);
	program->setVec3("camPosition", camera->getPosition());
	program->setVec3("material.albedo", albedo);
	program->setFloat("material.roughness", roughness);
	program->setFloat("material.metallic", metallic);
	program->setFloat("material.ao", ao);

	//irradianceMap
	auto ibl = renderer.getIBL();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, ibl->getIrradiancemap());
	program->setInt("irradianceMap", 0);

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