#include "window.h"
#include <iostream>
#include <functional>

void processInput(const Window& window, double deltaTime);

Window::Window(unsigned width, unsigned height, const std::shared_ptr<X_Renderer> renderer = nullptr) :screen_width(width), screen_height(height), last_mouseX(0.0f), last_mouseY(0.0f), isFirst(true), isHoldRightBtn(false), mWindow(nullptr), deltaTime(0.0),lastTime(0.0), renderer(renderer)
{
	//init
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	mWindow = glfwCreateWindow(screen_width, screen_height, "X-Renderer", nullptr, nullptr);
	glfwSetWindowUserPointer(mWindow, this);
	//create window
	if (mWindow == nullptr)
	{
		std::cout << "create glfw window failed" << std::endl;
		glfwTerminate();
	}
	glfwMakeContextCurrent(mWindow);

	if (!gladLoadGL((GLADloadfunc)glfwGetProcAddress))
	{
		std::cout << "failed to initialize GLAD!" << std::endl;
	}

	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glViewport(0, 0, screen_width, screen_height);

	auto size_callback = [](GLFWwindow* w, int width, int height) { static_cast<Window*>(glfwGetWindowUserPointer(w))->framebuffer_size_callback(width, height); };

	auto move_callback = [](GLFWwindow* w, double xpos, double ypos) { static_cast<Window*>(glfwGetWindowUserPointer(w))->mouseMove_callback(xpos, ypos); };

	auto click_callback = [](GLFWwindow* w, int button, int action, int modes) { static_cast<Window*>(glfwGetWindowUserPointer(w))->mouseClick_callback(button, action); };

	glfwSetFramebufferSizeCallback(mWindow, size_callback);
	glfwSetCursorPosCallback(mWindow, move_callback);
	glfwSetMouseButtonCallback(mWindow, click_callback);
}

Window::~Window(){ }

void Window::renderLoop(const SceneGraph& sceneGraph, RenderMode mode)
{
	/**enable-------------------------------------------*/
	glEnable(GL_DEPTH_TEST);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	/**-------------------------------------------------*/

	while (!glfwWindowShouldClose(mWindow))
	{
		double currentTime = glfwGetTime();
		deltaTime = currentTime - lastTime;		
		lastTime = currentTime;
		processInput(*this, deltaTime);
		clear();

		//render->render
		renderer->Render(sceneGraph, mode);

		glfwPollEvents();
		glfwSwapBuffers(mWindow);
	}
	glfwTerminate();
}

void Window::clear()
{
	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

#pragma region 事件回调

void Window::framebuffer_size_callback(int width, int height)
{
	//抛出事件
	getRenderer()->getCamera()->setRatio(width * 1.0f / height);
	glViewport(0, 0, width, height);
}

void Window::mouseMove_callback(double xpos, double ypos)
{
	if (isHoldRightBtn)
	{
		if (isFirst)
		{
			last_mouseX = xpos;
			last_mouseY = ypos;
			isFirst = false;
		}
		float xoffset = xpos - last_mouseX;
		float yoffset = ypos - last_mouseY;
		last_mouseX = xpos;
		last_mouseY = ypos;
		//抛出事件
		getRenderer()->getCamera()->pitchYaw(xoffset, yoffset);
	}
}

void Window::mouseClick_callback(int button, int action)
{
	if (button == 1 && action == 1)
	{
		isHoldRightBtn = true;
	}
	else {
		isHoldRightBtn = false;
		isFirst = true;
	}
}

void processInput(const Window& window, double deltaTime)
{
	if (glfwGetKey(window.getWindowInstance(), GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window.getWindowInstance(), true);
	}

	//抛出事件
	const auto keyConfig = window.getRenderer()->getCamera()->getKeybordConfig();
	for (auto& keyV : keyConfig)
	{
		if (glfwGetKey(window.getWindowInstance(), keyV.first) == GLFW_PRESS)
		{
			window.getRenderer()->getCamera()->move(keyV.second, deltaTime);
		}
	}
}


#pragma endregion

