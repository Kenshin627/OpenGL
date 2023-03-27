#pragma once
#include "../PostProcess.h"

class InversionEffect :public PostProcess
{
public:
	InversionEffect(const std::vector<std::string>& paths);
	~InversionEffect();
	void draw(unsigned slot) const override;
};