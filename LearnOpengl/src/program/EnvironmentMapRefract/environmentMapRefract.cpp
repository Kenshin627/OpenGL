#include "environmentMapRefract.h"

EnvironmentMapRefractShader::EnvironmentMapRefractShader(const std::vector<std::string> paths, std::shared_ptr<Camera> cam, unsigned skyBoxTextureID, float refractIndex) :
	Shader(paths),
	camera(cam),
	skyBoxTextureID(skyBoxTextureID),
	refractIndex(refractIndex) { }

void EnvironmentMapRefractShader::setCommonUniforms() const
{
	//camera
	setVec3("cameraPosition", camera->getPosition());

	//skyBox
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, skyBoxTextureID);
	setInt("skyBox", 0);
	setFloat("refractiveIndex", refractIndex);
}

void EnvironmentMapRefractShader::setMeshUniforms(std::shared_ptr<Mesh> mesh) const
{
	//mvp
	glm::mat4x4 modelMatrix = mesh->getModelMatrix();
	glm::mat3x3 normalMatrix = glm::mat3x3(glm::transpose(glm::inverse(modelMatrix)));
	setMatrix44("modelViewProjection", camera->projMatrix() * camera->viewMatrix() * modelMatrix);
	setMatrix44("model", modelMatrix);
	setMatrix33("normalMatrix", normalMatrix);	
}