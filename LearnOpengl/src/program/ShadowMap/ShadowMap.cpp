#include "ShadowMap.h"

ShadowMapShader::ShadowMapShader(const std::vector<std::string>& paths, std::shared_ptr<DirectionLight> light) :
	PostProcess(paths),
	light(light) {  }

void ShadowMapShader::setCommonUniforms() const
{
	setMatrix44("lightViewProjection", light->getLightSpaceMatrix());
}

void ShadowMapShader::setMeshUniforms(std::shared_ptr<Mesh> mesh) const
{
	setMatrix44("model", mesh->getModelMatrix());
}

void ShadowMapShader::bind() const
{
	glCullFace(GL_FRONT);
	glUseProgram(m_RendererID);
	//m_FBO->bind();
}

void ShadowMapShader::unbind() const
{
	glUseProgram(0);
	//m_FBO->unbind();
	glCullFace(GL_BACK);
}

void ShadowMapShader::buildFBO(unsigned viewportX, unsigned viewportY)
{
	m_FBO = std::make_shared<ShadowFrameBuffer>(viewportX, viewportY);
}