#include "../PostProcess.h"

class GrayScale :public PostProcess
{
public:
	GrayScale(const std::vector<std::string>& paths);
	void draw(unsigned slot) const override;
};