#pragma once
#include "../PostProcess.h"

class EdgeDetectionEffect :public PostProcess
{
public:
	EdgeDetectionEffect(const std::vector<std::string>& paths);
	~EdgeDetectionEffect();
	void draw(unsigned slot) const override;
};