#include <glad/gl.h>
#include "GlitchRGBSplit.h"

GlitchRGBSpliter::GlitchRGBSpliter(const std::string& name, const std::string& vertexShaderPath, const std::string& fragmentShaderPath, PostProcessMode mode, float speed = 10.0f, float intensity = 0.5f, Direction direct = Direction::Horizontal) :PostProcess(name, vertexShaderPath, fragmentShaderPath, mode), m_Speed(speed), m_Intensity(intensity * 0.1f), m_Direct(direct), m_Time(0.0f) {}

void GlitchRGBSpliter::setTime(float t)
{
	m_Time = (m_Time >= 100.0f) ? 0.0f : t;
}

void GlitchRGBSpliter::draw(unsigned slot) const
{
	program->setInt("pipeLineTexture", slot);
	program->setFloat("intensity", m_Intensity);
	program->setFloat("time", floorf(m_Time * m_Speed));
	program->setInt("direction", m_Direct);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (const void*)0);
}

void GlitchRGBSpliter::update(float ts) 
{
	setTime(ts);
}