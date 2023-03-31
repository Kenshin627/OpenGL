#include "../../mesh/Mesh.h"
#include "NormalMaterial.h"

NormalMaterial::NormalMaterial(const X_Renderer& renderer):
	Material(ShaderType::Normal, renderer) { }

void NormalMaterial::setUniforms(const Mesh& mesh)
{
	auto camera = renderer.getCamera();
	glm::mat4x4 model = mesh.getModelMatrix();
	glm::mat3x3 normalMatrix = glm::mat3x3(glm::transpose(glm::inverse(model)));
	glm::mat4x4 modelViewProjection = camera->projMatrix() * camera->viewMatrix() * model;
	program->setMatrix44("modelViewProjection", modelViewProjection);
	program->setMatrix33("normalMatrix", normalMatrix);
}
