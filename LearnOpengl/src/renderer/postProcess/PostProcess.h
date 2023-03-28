#pragma once
#include <memory>
#include <string>
#include "../../Buffer/FrameBuffer.h"
#include "../../program/Shader.h"

class PostProcess: public Shader
{
public:
	PostProcess(const std::vector<std::string>& paths);
	virtual ~PostProcess();

	void bind() const override;

	void unbind() const override;

	virtual void draw(unsigned slot) const;

	virtual void update(float ts);

	void resetFBO(unsigned viewporX, unsigned viewportY);

	virtual void buildFBO(unsigned viewportX, unsigned viewportY);
	
	std::shared_ptr<FrameBuffer> getFBO() const { return m_FBO; }
protected:
	std::shared_ptr<FrameBuffer> m_FBO;
};