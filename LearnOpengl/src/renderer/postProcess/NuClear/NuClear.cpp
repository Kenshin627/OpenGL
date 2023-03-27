#include <glad/gl.h>
#include "NuClear.h"

NuClear::NuClear(const std::string& name, const std::string& vertexShaderPath, const std::string& fragmentShaderPath, const std::string& geometryShaderPath) :PostProcess(name, vertexShaderPath, fragmentShaderPath, geometryShaderPath) {}

NuClear::~NuClear() {}
void NuClear::draw(unsigned slot) const
{
	program->setInt("pipeLineTexture", slot);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (const void*)0);
}