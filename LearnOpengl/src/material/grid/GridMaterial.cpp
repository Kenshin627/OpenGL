#include "kspch.h"
#include "../../mesh/Mesh.h"
#include "GridMaterial.h"

GridMaterial::GridMaterial(glm::vec3 mainColor, glm::vec3 lineColor, glm::vec3 gridOffset, glm::vec4 gridControl, const X_Renderer& renderer) :
	Material(ShaderType::Grid, renderer),
	mainColor(mainColor),
	lineColor(lineColor),
	gridOffset(gridOffset),
	gridControl(gridControl) {}

void GridMaterial::setUniforms(const Mesh& mesh)
{
	auto camera = renderer.getCamera();
	program->setVec3("mainColor", mainColor);
	program->setVec3("lineColor", lineColor);
	//gridRatio, majorUnitFrequency, minorUnitVisibility, opacity
	program->setVec3("gridOffset", gridOffset);
	program->setVec4("gridControl", gridControl);
	program->setMatrix44("modelViewProjection", camera->projMatrix() * camera->viewMatrix() * mesh.getModelMatrix());
}
