#include "GridCastShadow.h"

GridCastShadowShader::GridCastShadowShader(const std::vector<std::string> paths, std::shared_ptr<Camera> cam, std::shared_ptr<DirectionLight> light, std::shared_ptr<ShadowFrameBuffer> shadowBuffer) :
	Shader(paths),
	camera(cam),
	light(light),
	shadowBuffer(shadowBuffer) { }

void GridCastShadowShader::setCommonUniforms() const
{
	//directionLight
	setVec3("lightDirection", light->getDirection());

	//shadowMap
	glActiveTexture(GL_TEXTURE7);
	glBindTexture(GL_TEXTURE_2D, shadowBuffer->GetTextureID());
	setInt("shadowMap", 7);
	setMatrix44("lightPosSpace", light->getLightSpaceMatrix());
}

void GridCastShadowShader::setMeshUniforms(std::shared_ptr<Mesh> mesh) const
{
	//mvp
	glm::mat4x4 modelMatrix = mesh? mesh->getModelMatrix() : glm::identity<glm::mat4x4>();
	glm::mat3x3 normalMatrix = glm::mat3x3(glm::transpose(glm::inverse(modelMatrix)));
	setMatrix44("modelViewProjection", camera->projMatrix() * camera->viewMatrix() * modelMatrix);
	setMatrix44("model", modelMatrix);
	setMatrix33("normalMatrix", normalMatrix);	
}