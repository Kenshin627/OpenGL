#include "kspch.h"
#include "../../mesh/Mesh.h"
#include "WireFrameMaterial.h"

WireFrameMaterial::WireFrameMaterial(const X_Renderer& renderer, const glm::vec3& lineColor):
	Material(ShaderType::WireFrame, renderer),
	lineColor(lineColor){}

void WireFrameMaterial::setUniforms(const Mesh& mesh)
{
	auto camera = renderer.getCamera();
	program->setMatrix44("modelViewProjection", camera->projMatrix() * camera->viewMatrix() * mesh.getModelMatrix());
	program->setVec3("wireFrameColor", lineColor);
}
