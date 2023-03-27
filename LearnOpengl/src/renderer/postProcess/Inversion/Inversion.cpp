#include <glad/gl.h>
#include "Inversion.h"

InversionEffect::InversionEffect(const std::vector<std::string>& paths) :PostProcess(paths) {}

InversionEffect::~InversionEffect() {}
void InversionEffect::draw(unsigned slot) const
{
	setInt("pipeLineTexture", slot);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (const void*)0);
}