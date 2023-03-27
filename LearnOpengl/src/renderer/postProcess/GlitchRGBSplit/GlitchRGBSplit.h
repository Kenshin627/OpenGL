#include "../PostProcess.h"

enum Direction
{
	Horizontal,
	vertical,
	Horizontal_Vertical
};

class GlitchRGBSpliter : public PostProcess
{
public:
	GlitchRGBSpliter(const std::string& name, const std::string& vertexShaderPath, const std::string& fragmentShaderPath, const std::string& geometryShaderPath, float speed, float intensity, Direction direct);
	void draw(unsigned slot) const override;
	void update(float ts) override;
	void setTime(float t);
	void setSpeed(float s) { m_Speed = s; };
	void setIntensity(float i) { m_Intensity = i; };
private:
	float m_Speed;
	//[-1.0f - 1.0f]
	float m_Intensity;
	Direction m_Direct;
	//[0.0f - 100.0f]
	float m_Time;
};
