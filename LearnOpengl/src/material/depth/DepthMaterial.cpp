#include "kspch.h"
#include "../../mesh/Mesh.h"
#include "DepthMaterial.h"

DepthMaterial::DepthMaterial(const X_Renderer& renderer, const glm::vec3& nearColor, const glm::vec3& farColor):
	Material(ShaderType::Depth, renderer),
	nearColor(nearColor),
	farColor(farColor) {}

void DepthMaterial::setUniforms(const Mesh& mesh)
{
	auto camera = renderer.getCamera();
	program->setFloat("near", camera->getNear());
	program->setFloat("far", camera->getFar());
	program->setVec3("nearColor", nearColor);
	program->setVec3("farColor", farColor);
	program->setMatrix44("modelViewProjection", camera->projMatrix() * camera->viewMatrix() * mesh.getModelMatrix());
}
