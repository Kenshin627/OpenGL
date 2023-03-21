#pragma once

extern Kenshin::Application* Kenshin::createApplication(int argc, char** argv);

bool g_ApplicationRunning = true;

namespace Kenshin
{
	int main(int argc, char** argv)
	{
		while (g_ApplicationRunning)
		{
			Application* app = Kenshin::createApplication(argc, argv);
			app->run();
			delete app;
		}
		return 0;
	}
}

int main(int argc, char** argv)
{
	return Kenshin::main(argc, argv);
}