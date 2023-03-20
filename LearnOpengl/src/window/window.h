#pragma once
#include "../renderer/Renderer.h"
#include <GLFW/glfw3.h>
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
	const std::shared_ptr<X_Renderer>& getRenderer() const { return renderer; };
	void framebuffer_size_callback(int width, int height);
	void mouseMove_callback(double xpos, double ypos);
	void mouseClick_callback(int button, int action);
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

