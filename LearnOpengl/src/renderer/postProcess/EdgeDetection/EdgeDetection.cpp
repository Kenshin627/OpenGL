#include <glad/gl.h>
#include "EdgeDetection.h"

EdgeDetection::EdgeDetection(const std::string& name, const std::string& vertexShaderPath, const std::string& fragmentShaderPath, PostProcessMode mode) :PostProcess(name, vertexShaderPath, fragmentShaderPath, mode) {}

EdgeDetection::~EdgeDetection() {}
void EdgeDetection::draw(unsigned slot) const
{
	program->setInt("pipeLineTexture", slot);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (const void*)0);
}