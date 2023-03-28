#include "PostProcess.h"

PostProcess::PostProcess(const std::vector<std::string>& paths, std::shared_ptr<FrameBuffer> prev_buffer) :
	Shader(paths),
	prev_FBO(prev_buffer)
{
	buildFBO(1, 1);
	type = ShaderCategory::PostProcessShader;
}

PostProcess::~PostProcess() = default;

void PostProcess::bind() const
{
	glUseProgram(m_RendererID);
	m_FBO->bind();
}

void PostProcess::unbind() const
{
	m_FBO->unbind();
	glUseProgram(0);
}

void PostProcess::draw(unsigned slot) const
{}

void PostProcess::update(float ts) {}

void PostProcess::resetFBO(unsigned viewportX, unsigned viewportY)
{
	m_FBO->resize(viewportX, viewportY);
}

void PostProcess::buildFBO(unsigned viewportX, unsigned viewportY)
{
	m_FBO = std::make_shared<FrameBuffer>(viewportX, viewportY);
}