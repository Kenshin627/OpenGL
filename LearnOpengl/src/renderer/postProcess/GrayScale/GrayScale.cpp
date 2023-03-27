#include <glad/gl.h>
#include "GrayScale.h"

GrayScale::GrayScale(const std::vector<std::string>& paths) :PostProcess(paths) { }

void GrayScale::draw(unsigned slot) const
{
	setInt("pipeLineTexture", slot);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (const void*)0);
}