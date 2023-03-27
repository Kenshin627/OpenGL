#include "WireFrame.h"

WireFrameShader::WireFrameShader(const std::vector<std::string> paths, std::shared_ptr<Camera> cam, glm::vec3& wireFrameColor):
	Shader(paths),
	camera(cam),wireFrameColor(wireFrameColor)
	{  }

void WireFrameShader::setCommonUniforms() const
{
	setVec3("wireFrameColor", wireFrameColor);
}

void WireFrameShader::setMeshUniforms(std::shared_ptr<Mesh> mesh) const
{
	//mvp
	glm::mat4x4 modelMatrix = mesh->getModelMatrix();
	setMatrix44("modelViewProjection", camera->projMatrix() * camera->viewMatrix() * modelMatrix);
}

void WireFrameShader::bind() const
{
	glUseProgram(m_RendererID);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

void WireFrameShader::unbind() const
{
	glUseProgram(0);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}