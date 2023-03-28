#include "VisualNormal.h"

VisualNormalShader::VisualNormalShader(const std::vector<std::string>& paths, std::shared_ptr<Camera> cam, float magnitude, glm::vec3 lineColor):
	Shader(paths),
	camera(cam), 
	lineMagnitude(magnitude), 
	lineColor(lineColor) {  }

void VisualNormalShader::setCommonUniforms() const
{
	setFloat("magnitude", lineMagnitude);
	setVec3("lineColor", lineColor);
}

void VisualNormalShader::setMeshUniforms(std::shared_ptr<Mesh> mesh) const
{
	glm::mat4x4 modelView = camera->viewMatrix() * mesh->getModelMatrix();
	glm::mat3x3 modelViewInverseTranspose = glm::mat3x3(glm::transpose(glm::inverse(modelView)));
	setMatrix44("modelView", modelView);
	setMatrix33("inverseModelView", modelViewInverseTranspose);
	setMatrix44("projection", camera->projMatrix());
}