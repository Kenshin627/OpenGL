#pragma once
#include <memory>
#include <string>
#include "../../Buffer/FrameBuffer.h"
#include "../../program/Shader.h"

class PostProcess: public Shader
{
public:
	PostProcess(const std::vector<std::string>& paths, std::shared_ptr<FrameBuffer> prev_buffer);
	virtual ~PostProcess();

	virtual void bind() const override;

	virtual void unbind() const override;

	virtual void draw(unsigned slot) const;

	virtual void update(float ts);

	void resetFBO(unsigned viewporX, unsigned viewportY);

	virtual void buildFBO(unsigned viewportX, unsigned viewportY);
	
	std::shared_ptr<FrameBuffer> getFBO() const override { return m_FBO; }
	std::shared_ptr<FrameBuffer> getPrevFBO() const override { return prev_FBO; };
protected:
	std::shared_ptr<FrameBuffer> m_FBO;
	std::shared_ptr<FrameBuffer> prev_FBO;
};