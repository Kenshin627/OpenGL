#pragma once
#include <memory>
#include <string>

#include "../../Buffer/FrameBuffer.h"
#include "../../program/Shader.h"

class PostProcess
{
public:
	PostProcess(const std::string& name, const std::string& vertexShaderPath, const std::string& fragmentShaderPath, const std::string& geometryShaderPath = "") :
		name(name),
		program(std::make_shared<Shader>(vertexShaderPath, fragmentShaderPath)),
		m_FBO(std::make_shared<FrameBuffer>(1, 1))
	{ }

	void bind() const
	{
		program->bind();
		m_FBO->bind();
	}

	void unbind() const
	{
		m_FBO->unbind();
		program->unbind();
	}

	virtual void draw(unsigned slot) const { }

	virtual void update(float ts) { }

	void resetFBO(unsigned viewporX, unsigned viewportY)
	{
		m_FBO->resize(viewporX, viewportY);
	}

	std::shared_ptr<FrameBuffer> getFBO() const { return m_FBO; };
protected:
	std::string name;
	std::shared_ptr<Shader> program;
	std::shared_ptr<FrameBuffer> m_FBO;
};