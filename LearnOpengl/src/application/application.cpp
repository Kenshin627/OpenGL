#include "application.h"
#include "vendor/imGui/imgui_impl_glfw.h"
#include "vendor/imGui/imgui_impl_opengl3.h"
#include <glm/glm.hpp>
#include <iostream>

#include <glad/gl.h>
#include <GLFW/glfw3.h>

extern bool g_ApplicationRunning;

static Kenshin::Application* s_Instance = nullptr;

namespace Kenshin
{
	Application::Application(const ApplicationSpecification& specification) :m_Specification(specification), m_MenubarCallback(nullptr), m_Running(false), m_TimeStep(0.0f), m_FrameTime(0.0f), m_LastFrameTime(0.0f), m_windowHandle(nullptr), clearColor(ImVec4(0.2, 0.2, 0.2, 1.0)), isHoldRightBtn(false), isFirst(true), last_mouseX(0.0f), last_mouseY(0.0f)
	{
		s_Instance = this;
		init();
	}

	Application::~Application()
	{
		shutdown();
		s_Instance = nullptr;
	}

	Application& Application::Get() { return *s_Instance; };

	void Application::init()
	{
		//window
		glfwInit();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		m_windowHandle = glfwCreateWindow(m_Specification.width, m_Specification.height, m_Specification.name.c_str(), nullptr, nullptr);
		glfwSetWindowUserPointer(m_windowHandle, this);
		//create window
		if (m_windowHandle == nullptr)
		{
			std::cout << "create glfw window failed" << std::endl;
			glfwTerminate();
		}
		glfwMakeContextCurrent(m_windowHandle);

		//imGui
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
		//io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
		//io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
		ImGui::StyleColorsDark();
		ImGui_ImplGlfw_InitForOpenGL(m_windowHandle, true);
		ImGui_ImplOpenGL3_Init("#version 460");

		if (!gladLoadGL((GLADloadfunc)glfwGetProcAddress))
		{
			std::cout << "failed to initialize GLAD!" << std::endl;
		}

		//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		//glViewport(0, 0, m_Specification.width, m_Specification.height);

		//ImFontConfig fontConfig;
		//fontConfig.FontDataOwnedByAtlas = false;
		//ImFont* robotoFont = io.Fonts->AddFontFromMemoryTTF((void*)g_RobotoRegular, sizeof(g_RobotoRegular), 20.0f, &fontConfig);
		//io.FontDefault = robotoFont;

		auto size_callback = [](GLFWwindow* w, int width, int height) { static_cast<Application*>(glfwGetWindowUserPointer(w))->framebuffer_size_callback(width, height); };

		auto move_callback = [](GLFWwindow* w, double xpos, double ypos) { static_cast<Application*>(glfwGetWindowUserPointer(w))->mouseMove_callback(xpos, ypos); };

		auto click_callback = [](GLFWwindow* w, int button, int action, int modes) { static_cast<Application*>(glfwGetWindowUserPointer(w))->mouseClick_callback(button, action); };

		glfwSetFramebufferSizeCallback(m_windowHandle, size_callback);
		glfwSetCursorPosCallback(m_windowHandle, move_callback);
		glfwSetMouseButtonCallback(m_windowHandle, click_callback);
	}

	void Application::shutdown()
	{
		for (auto& layer : m_LayerSatack)
		{
			layer->onDetach();
		}

		m_LayerSatack.clear();

		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();

		glfwDestroyWindow(m_windowHandle);
		glfwTerminate();
		g_ApplicationRunning = false;
	}

	void Application::run()
	{
		m_Running = true;
		glEnable(GL_DEPTH_TEST);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		ImGuiIO& io = ImGui::GetIO();

		while (!glfwWindowShouldClose(m_windowHandle) && m_Running)
		{
			glfwPollEvents();
			//processInput(*this, deltaTime);

			/*ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();*/

			//ImGui::Begin("X_Renderer");           
			for (auto& layer : m_LayerSatack)
			{
				layer->onUIRender();
			}
			//ImGui::End();

			/*ImGui::Render();*/

			for (auto& layer : m_LayerSatack)
			{
				layer->onUpdate(m_TimeStep);
			}

			//renderer->Render(sceneGraph, mode);
			
			int display_w, display_h;
			glfwGetFramebufferSize(m_windowHandle, &display_w, &display_h);			
			glViewport(0, 0, display_w, display_h);
			glClearColor(clearColor.x, clearColor.y, clearColor.z, clearColor.w);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			//ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());			
			glfwSwapBuffers(m_windowHandle);

			setTime();
			//clear();
		}
	}

	void Application::close()
	{
		m_Running = false;
	}

	float Application::getTime() const
	{
		return (float)glfwGetTime();
	}

	void Application::setTime()
	{
		double currentTime = getTime();
		m_FrameTime = currentTime - m_LastFrameTime;
		m_LastFrameTime = currentTime;
		m_TimeStep = std::min<float>(m_FrameTime, 0.0333f);
	}

	#pragma region 事件回调

	void Application::framebuffer_size_callback(int width, int height)
	{
		//抛出事件
		//getRenderer()->getCamera()->setRatio(width * 1.0f / height);
		glViewport(0, 0, width, height);
	}

	void Application::mouseMove_callback(double xpos, double ypos)
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
			//getRenderer()->getCamera()->pitchYaw(xoffset, yoffset);
		}
	}

	void Application::mouseClick_callback(int button, int action)
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

	void processInput(const Application& app, double deltaTime)
	{
		GLFWwindow* handle = app.getWindowHandle();
		if (glfwGetKey(handle, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		{
			glfwSetWindowShouldClose(handle, true);
		}

		//抛出事件
		//const auto keyConfig = window.getRenderer()->getCamera()->getKeybordConfig();
		//for (auto& keyV : keyConfig)
		//{
		//	if (glfwGetKey(window.getWindowInstance(), keyV.first) == GLFW_PRESS)
		//	{
		//		window.getRenderer()->getCamera()->move(keyV.second, deltaTime);
		//	}
		//}
	}

	#pragma endregion

}

