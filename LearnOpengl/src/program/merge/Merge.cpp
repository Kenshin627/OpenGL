#include "Merge.h"

MergeShader::MergeShader(const std::vector<std::string> paths, std::shared_ptr<FrameBuffer> pipelineA, std::shared_ptr<FrameBuffer> pipelineB) :
	Shader(paths),
	pipelineA(pipelineA),
	pipelineB(pipelineB) {}

void MergeShader::setCommonUniforms() const
{
	glActiveTexture(GL_TEXTURE0 + 8);
	glBindTexture(GL_TEXTURE_2D, pipelineA->GetTextureID());
	setInt("pipelineA", 8);

	glActiveTexture(GL_TEXTURE0 + 9);
	glBindTexture(GL_TEXTURE_2D, pipelineB->GetTextureID());
	setInt("pipelineB", 9);
}

void MergeShader::setMeshUniforms(std::shared_ptr<Mesh> mesh) const
{
	glUseProgram(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}