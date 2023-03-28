#include <glad/gl.h>
#include "EdgeDetection.h"

EdgeDetectionShader::EdgeDetectionShader(const std::vector<std::string>& paths, std::shared_ptr<FrameBuffer> prev_buffer) :PostProcess(paths, prev_buffer) {}

EdgeDetectionShader::~EdgeDetectionShader() {}

void EdgeDetectionShader::setCommonUniforms() const
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, prev_FBO->GetTextureID());
	setInt("pipeLineTexture", 0);	
}

void EdgeDetectionShader::unbind() const
{
	glBindTexture(GL_TEXTURE_2D, 0);
}