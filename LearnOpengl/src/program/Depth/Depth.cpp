#include "Depth.h"

DepthShader::DepthShader(const std::vector<std::string> paths, std::shared_ptr<Camera> cam):
	Shader(paths),
	camera(cam) {  }

void DepthShader::setCommonUniforms() const
{
	setFloat("near", camera->getNear());
	setFloat("far", camera->getFar());
}

void DepthShader::setMeshUniforms(std::shared_ptr<Mesh> mesh) const
{
	//mvp
	glm::mat4x4 modelMatrix = mesh->getModelMatrix();
	setMatrix44("modelViewProjection", camera->projMatrix() * camera->viewMatrix() * modelMatrix);
}