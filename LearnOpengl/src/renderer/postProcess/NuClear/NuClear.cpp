#include <glad/gl.h>
#include "NuClear.h"

NuClearEffect::NuClearEffect(const std::vector<std::string>& paths) :PostProcess(paths) {}

NuClearEffect::~NuClearEffect() {}
void NuClearEffect::draw(unsigned slot) const
{
	setInt("pipeLineTexture", slot);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (const void*)0);
}