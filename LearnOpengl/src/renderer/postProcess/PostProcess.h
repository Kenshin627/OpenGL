#pragma once
#include <memory>
#include <string>

#include "../../Buffer/FrameBuffer.h"
#include "../../program/Shader.h"

class PostProcess: public Shader
{
public:
	PostProcess(const std::vector<std::string>& paths) :
		Shader(paths)
	{ 
		buildFBO(1.0f, 1.0f);
		type = ShaderCategory::PostProcessShader;
	}
	virtual ~PostProcess() {}

	void bind() const override
	{
		glUseProgram(m_RendererID);
		m_FBO->bind();
	}

	void unbind() const override
	{
		m_FBO->unbind();
		glUseProgram(0);
	}

	virtual void draw(unsigned slot) const { }

	virtual void update(float ts) { }

	void resetFBO(unsigned viewporX, unsigned viewportY)
	{
		m_FBO->resize(viewporX, viewportY);
	}

	virtual void buildFBO(unsigned viewportX, unsigned viewportY)
	{
		m_FBO = std::make_shared<FrameBuffer>(viewportX, viewportY);
	}

	std::shared_ptr<FrameBuffer> getFBO() const { return m_FBO; }
protected:
	std::shared_ptr<FrameBuffer> m_FBO;
};