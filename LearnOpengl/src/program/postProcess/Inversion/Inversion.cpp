#include <glad/gl.h>
#include "Inversion.h"

InversionShader::InversionShader(const std::vector<std::string>& paths, std::shared_ptr<FrameBuffer> prevbuffer) :PostProcess(paths, prevbuffer) {}

InversionShader::~InversionShader() {}

void InversionShader::setCommonUniforms() const
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, prev_FBO->GetTextureID());
	setInt("pipeLineTexture", 0);
}

void InversionShader::unbind() const
{
	glBindTexture(GL_TEXTURE_2D, 0);
}