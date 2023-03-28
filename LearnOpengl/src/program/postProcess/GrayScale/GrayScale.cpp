#include <glad/gl.h>
#include "GrayScale.h"

GrayScaleShader::GrayScaleShader(const std::vector<std::string>& paths, std::shared_ptr<FrameBuffer> prevbuffer) :PostProcess(paths, prevbuffer) { }

void GrayScaleShader::setCommonUniforms() const
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, prev_FBO->GetTextureID());
	setInt("pipeLineTexture", 0);
}

void GrayScaleShader::unbind() const
{
	m_FBO->unbind();
	glUseProgram(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}