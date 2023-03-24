#include <glad/gl.h>
#include "GrayScale.h"


GrayScale::GrayScale(const std::string& name, const std::string& vertexShaderPath, const std::string& fragmentShaderPath, PostProcessMode mode) :PostProcess(name, vertexShaderPath, fragmentShaderPath, mode) { }

void GrayScale::draw(unsigned slot) const
{
	program->setInt("pipeLineTexture", slot);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (const void*)0);
}