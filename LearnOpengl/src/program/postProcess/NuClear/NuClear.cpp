#include <glad/gl.h>
#include "NuClear.h"

NuClearShader::NuClearShader(const std::vector<std::string>& paths, std::shared_ptr<FrameBuffer> prevbuffer) :PostProcess(paths, prevbuffer) {}

NuClearShader::~NuClearShader() {}

void NuClearShader::setCommonUniforms() const
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, prev_FBO->GetTextureID());
	setInt("pipeLineTexture", 0);
}

void NuClearShader::unbind() const
{
	glBindTexture(GL_TEXTURE_2D, 0);
}