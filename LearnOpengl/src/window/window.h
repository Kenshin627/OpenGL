#pragma once
#include "../renderer/Renderer.h"
#include <glfw3.h>
#include <memory>

class Window
{
public:
	Window(unsigned width, unsigned height, std::shared_ptr<X_Renderer> renderer);
	~Window();
	void renderLoop(const SceneGraph& sceneGraph, RenderMode mode);
	void clear();
	float getDeltaTime() const { return deltaTime; };
	GLFWwindow* getWindowInstance()const { return mWindow; };
private:
private:
	int screen_width;
	int screen_height;
	float last_mouseX;
	float last_mouseY;
	float isFirst;
	bool isHoldRightBtn;
	GLFWwindow* mWindow;
	double deltaTime;
	double lastTime;
	std::shared_ptr<X_Renderer> renderer;
};

