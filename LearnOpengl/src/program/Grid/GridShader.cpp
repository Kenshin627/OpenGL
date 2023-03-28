#include "GridShader.h"

GridShader::GridShader(const std::vector<std::string>& paths, std::shared_ptr<Camera> cam, glm::vec3 mainColor, glm::vec3 lineColor, glm::vec3 gridOffset, glm::vec4 gridControl):
	Shader(paths),
	camera(cam),
	mainColor(mainColor),
	lineColor(lineColor),
	gridOffset(gridOffset),
	gridControl(gridControl) {  }

void GridShader::setCommonUniforms() const
{
	setVec3("mainColor", mainColor);
	setVec3("lineColor", lineColor);
	//gridRatio, majorUnitFrequency, minorUnitVisibility, opacity
	setVec3("gridOffset", gridOffset);
	setVec4("gridControl", gridControl);
}

void GridShader::setMeshUniforms(std::shared_ptr<Mesh> mesh) const
{
	setMatrix44("modelViewProjection", camera->projMatrix() * camera->viewMatrix() * mesh->getModelMatrix());
}