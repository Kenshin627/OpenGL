#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "window.h"
#include <iostream>
#include <functional>

void processInput(const Window& window, double deltaTime);

Window::Window(unsigned width, unsigned height, const std::shared_ptr<X_Renderer> renderer = nullptr) :screen_width(width), screen_height(height), last_mouseX(0.0f), last_mouseY(0.0f), isFirst(true), isHoldRightBtn(false), mWindow(nullptr), deltaTime(0.0),lastTime(0.0), renderer(renderer)
{

}

Window::~Window(){ }

void Window::renderLoop(const SceneGraph& sceneGraph, RenderMode mode)
{
	/**enable-------------------------------------------*/
	
	/**-------------------------------------------------*/

	while (!glfwWindowShouldClose(mWindow))
	{
		glfwPollEvents();

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();


		ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.
		ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
		ImGui::End();

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		double currentTime = glfwGetTime();
		deltaTime = currentTime - lastTime;		
		lastTime = currentTime;
		processInput(*this, deltaTime);
		clear();

		//render->render
		renderer->Render(sceneGraph, mode);

		glfwSwapBuffers(mWindow);
	}

	
}

void Window::clear()
{
	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}



