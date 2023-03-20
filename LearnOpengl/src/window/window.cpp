#include "window.h"
#include <iostream>

void processInput(const Window& window);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

Window::Window(unsigned width, unsigned height, const std::shared_ptr<X_Renderer> renderer = nullptr) :screen_width(width), screen_height(height), last_mouseX(0.0f), last_mouseY(0.0f), isFirst(true), isHoldRightBtn(false), mWindow(nullptr), deltaTime(0.0),lastTime(0.0), renderer(renderer)
{
	//init
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//create window
	mWindow = glfwCreateWindow(screen_width, screen_height, "X-Renderer", nullptr, nullptr);
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
	glfwSetFramebufferSizeCallback(mWindow, framebuffer_size_callback);
	//glfwSetCursorPosCallback(mWindow.get(), mouseMove_callback);
	//glfwSetMouseButtonCallback(mWindow.get(), mouseClick_callback);
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
		processInput(*this);
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

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	//抛出事件
	//camera.setRatio(width * 1.0f / height);
	glViewport(0, 0, width, height);
}

//void mouseMove_callback(GLFWwindow* window, double xpos, double ypos)
//{
//	if (isHoldRightBtn)
//	{
//		if (isFirst)
//		{
//			last_mouseX = xpos;
//			last_mouseY = ypos;
//			isFirst = false;
//		}
//		float xoffset = xpos - last_mouseX;
//		float yoffset = ypos - last_mouseY;
//		last_mouseX = xpos;
//		last_mouseY = ypos;
//		//抛出事件
//		//camera.pitchYaw(xoffset, yoffset);
//	}
//}
//
//void mouseClick_callback(GLFWwindow* window, int button, int action, int modes)
//{
//	if (button == 1 && action == 1)
//	{
//		isHoldRightBtn = true;
//	}
//	else {
//		isHoldRightBtn = false;
//		isFirst = true;
//	}
//}

void processInput(const Window& window)
{	
	if (glfwGetKey(window.getWindowInstance(), GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window.getWindowInstance(), true);
	}

	//抛出事件
	/*const auto keyConfig = camera.getKeybordConfig();
	for (auto& keyV : keyConfig)
	{
		if (glfwGetKey(window, keyV.first) == GLFW_PRESS)
		{
			camera.move(keyV.second, deltaTime);
		}
	}*/
}

