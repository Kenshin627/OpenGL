#include "BlinnPhongCastShadow.h"

BlinnPhongCastShadowShader::BlinnPhongCastShadowShader(const std::vector<std::string> paths, std::shared_ptr<Camera> cam, std::shared_ptr<DirectionLight> light, std::shared_ptr<ShadowFrameBuffer> shadowFrameBuffer) :
	Shader(paths),
	camera(cam),
	light(light),
	shadowFrameBuffer(shadowFrameBuffer) { }

void BlinnPhongCastShadowShader::setCommonUniforms() const
{
	//directionLight
	setVec3("directionLight.color", light->getColor());
	setVec3("directionLight.direction", light->getDirection());
	
	//camera
	setVec3("cameraPosition", camera->getPosition());

	glActiveTexture(GL_TEXTURE6);
	glBindTexture(GL_TEXTURE_2D, shadowFrameBuffer->GetTextureID());
	setInt("shadowMap", 6);
	setMatrix44("lightPosSpace", light->getLightSpaceMatrix());
}

void BlinnPhongCastShadowShader::setMeshUniforms(std::shared_ptr<Mesh> mesh) const
{
	//mvp
	glm::mat4x4 modelMatrix = mesh->getModelMatrix();
	glm::mat3x3 normalMatrix = glm::mat3x3(glm::transpose(glm::inverse(modelMatrix)));
	setMatrix44("modelViewProjection", camera->projMatrix() * camera->viewMatrix() * modelMatrix);
	setMatrix44("model", modelMatrix);
	setMatrix33("normalMatrix", normalMatrix);

	//material
	auto mat = mesh->getMaterial();
	if (!mat->ambientTextures.empty())
	{
		mat->ambientTextures[0]->bind(0);
		setInt("material.ambientTexture", 0);
	}
	if (!mat->diffuseTextures.empty())
	{
		mat->diffuseTextures[0]->bind(0);
		setInt("material.diffuseTexture", 0);
	}
	if (!mat->specularTextures.empty())
	{
		mat->specularTextures[0]->bind(2);
		setInt("material.specularTexture", 2);
	}
	setFloat("material.shininess", mat->shininess ? mat->shininess : 64.0);
}