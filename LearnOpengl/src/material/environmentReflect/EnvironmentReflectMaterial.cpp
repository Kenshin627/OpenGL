#include <glad/gl.h>
#include "../../mesh/Mesh.h"
#include "EnvironmentReflectMaterial.h"

EnvironmentReflectMaterial::EnvironmentReflectMaterial(const X_Renderer& renderer):
	Material(ShaderType::EnvironmentMapReflect, renderer) { }

void EnvironmentReflectMaterial::setUniforms(const Mesh& mesh)
{
	auto camera = renderer.getCamera();
	//camera
	program->setVec3("cameraPosition", camera->getPosition());

	//skyBox
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, renderer.getSkyBoxTexture());
	program->setInt("skyBox", 0);

	//mvp
	
	glm::mat4x4 modelMatrix = mesh.getModelMatrix();
	glm::mat3x3 normalMatrix = glm::mat3x3(glm::transpose(glm::inverse(modelMatrix)));
	program->setMatrix44("modelViewProjection", camera->projMatrix() * camera->viewMatrix() * modelMatrix);
	program->setMatrix44("model", modelMatrix);
	program->setMatrix33("normalMatrix", normalMatrix);
}
