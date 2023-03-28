#include <glad/gl.h>
#include "GlitchRGBSplit.h"

GlitchRGBSpliter::GlitchRGBSpliter(const std::vector<std::string>& paths, std::shared_ptr<FrameBuffer> prevBuffer = nullptr, float speed = 10.0f, float intensity = 0.5f, Direction direct = Direction::Horizontal) :PostProcess(paths, prevBuffer), m_Speed(speed), m_Intensity(intensity * 0.1f), m_Direct(direct), m_Time(0.0f) {}

void GlitchRGBSpliter::setTime(float t)
{
	m_Time = (m_Time >= 100.0f) ? 0.0f : t;
}


void GlitchRGBSpliter::setCommonUniforms() const
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, prev_FBO->GetTextureID());
	setInt("pipeLineTexture", 0);
	setFloat("intensity", m_Intensity);
	setFloat("time", floorf(m_Time * m_Speed));
	setInt("direction", m_Direct);
}

void GlitchRGBSpliter::unbind() const
{
	glBindTexture(GL_TEXTURE_2D, 0);
}

void GlitchRGBSpliter::update(float ts) 
{
	setTime(ts);
}