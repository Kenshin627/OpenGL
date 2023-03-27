#include <glad/gl.h>
#include "GlitchRGBSplit.h"

GlitchRGBSpliter::GlitchRGBSpliter(const std::vector<std::string>& paths, float speed = 10.0f, float intensity = 0.5f, Direction direct = Direction::Horizontal) :PostProcess(paths), m_Speed(speed), m_Intensity(intensity * 0.1f), m_Direct(direct), m_Time(0.0f) {}

void GlitchRGBSpliter::setTime(float t)
{
	m_Time = (m_Time >= 100.0f) ? 0.0f : t;
}

void GlitchRGBSpliter::draw(unsigned slot) const
{
	setInt("pipeLineTexture", slot);
	setFloat("intensity", m_Intensity);
	setFloat("time", floorf(m_Time * m_Speed));
	setInt("direction", m_Direct);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (const void*)0);
}

void GlitchRGBSpliter::update(float ts) 
{
	setTime(ts);
}