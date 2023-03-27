#include <glad/gl.h>
#include "EdgeDetection.h"

EdgeDetectionEffect::EdgeDetectionEffect(const std::vector<std::string>& paths) :PostProcess(paths) {}

EdgeDetectionEffect::~EdgeDetectionEffect() {}
void EdgeDetectionEffect::draw(unsigned slot) const
{
	setInt("pipeLineTexture", slot);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (const void*)0);
}