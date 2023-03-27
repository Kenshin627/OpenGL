#pragma once
#include "../PostProcess.h"

class NuClearEffect :public PostProcess
{
public:
	NuClearEffect(const std::vector<std::string>& paths);
	~NuClearEffect();
	void draw(unsigned slot) const override;
};