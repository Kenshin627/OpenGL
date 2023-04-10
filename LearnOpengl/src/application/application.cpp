#include "kspch.h"
#include "application.h"
#include "vendor/imGui/imgui_impl_glfw.h"
#include "vendor/imGui/imgui_impl_opengl3.h"
#include <glad/gl.h>
#include <GLFW/glfw3.h>

extern bool g_ApplicationRunning;

namespace Kenshin
{
	Application* s_Instance = nullptr;
	Application::Application(const ApplicationSpecification& specification) :m_Specification(specification), m_MenubarCallback(nullptr), m_Running(false), m_TimeStep(0.0f), m_FrameTime(0.0f), m_LastFrameTime(0.0f), m_windowHandle(nullptr), isHoldRightBtn(false), isFirst(true), last_mouseX(0.0f), last_mouseY(0.0f), m_viewportSize(glm::vec2(0.0f, 0.0f))
	{
		s_Instance = this;
		init();
	}

	Application::~Application()
	{
		shutdown();
		uiLayer.onDetach();
		s_Instance = nullptr;
	}

	void Application::init()
	{
		glfwInit();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		m_windowHandle = glfwCreateWindow(m_Specification.width, m_Specification.height, m_Specification.name.c_str(), nullptr, nullptr);		
		if (m_windowHandle == nullptr)
		{
			std::cout << "create glfw window failed" << std::endl;
			glfwTerminate();
		}
		glfwMakeContextCurrent(m_windowHandle);

		if (!gladLoadGL((GLADloadfunc)glfwGetProcAddress))
		{
			std::cout << "failed to initialize GLAD!" << std::endl;
		}
		uiLayer.onAttach();
	}

	void Application::shutdown()
	{
		for (auto& layer : m_LayerStack)
		{
			layer->onDetach();
		}
		glfwDestroyWindow(m_windowHandle);
		glfwTerminate();
		g_ApplicationRunning = false;
	}

	void Application::run()
	{
		m_Running = true;		
		ImGuiIO& io = ImGui::GetIO();
		while (!glfwWindowShouldClose(m_windowHandle) && m_Running)
		{
			glfwPollEvents();			
			ImGuiLayer::begin();

			static bool opt_fullscreen = true;
			static bool opt_padding = false;
			static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;
			ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;

			if (opt_fullscreen)
			{
				const ImGuiViewport* viewport = ImGui::GetMainViewport();
				ImGui::SetNextWindowPos(viewport->WorkPos);
				ImGui::SetNextWindowSize(viewport->WorkSize);
				ImGui::SetNextWindowViewport(viewport->ID);
				ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
				ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
				window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
				window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
			}
			else
			{
				dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
			}
			if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
				window_flags |= ImGuiWindowFlags_NoBackground;
			if (!opt_padding)
				ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
			ImGui::Begin("DockSpace Demo", nullptr, window_flags);
			if (!opt_padding)
				ImGui::PopStyleVar();

			if (opt_fullscreen)
				ImGui::PopStyleVar(2);
			if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
			{
				ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
				ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
			}
			if (ImGui::BeginMenuBar())
			{
				m_MenubarCallback();
				ImGui::EndMenuBar();
			}
						

			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
			ImGui::Begin("Scene");
			ImGuiIO io = ImGui::GetIO();
			ImVec2 size = ImGui::GetContentRegionAvail();			
			
			for (auto& layer : m_LayerStack)
			{
				layer->onUpdate(Kenshin::updatePayload{ glm::vec2(size.x, size.y), m_FrameTime, io, ImGui::IsWindowHovered(), ImGui::IsWindowFocused(), getTime()});
			}
			ImGui::End();
			ImGui::PopStyleVar();

			for (auto& layer : m_LayerStack)
			{
				layer->onUIRender(getFPS(), getFrameTime());
			}

			ImGui::End();
		
			ImGuiLayer::end();
			glfwSwapBuffers(m_windowHandle);
			setTime();
		}
	}

	void Application::PushLayer(Layer* layer)
	{
		layer->onAttach();
		m_LayerStack.PushLayer(layer);
	}

	void Application::PushOverlay(Layer* overLay)
	{
		overLay->onAttach();
		m_LayerStack.PushOverLay(overLay);
	}

	void Application::close()
	{
		m_Running = false;
	}

	float Application::getTime() const
	{
		return (float)glfwGetTime();
	}

	float Application::getFrameTime() const
	{
		return m_FrameTime;
	}

	float Application::getFPS() const
	{
		return 1.0f / getFrameTime();
	}

	void Application::setTime()
	{
		float currentTime = getTime();
		m_FrameTime = currentTime - m_LastFrameTime;
		m_LastFrameTime = currentTime;
		m_TimeStep = std::min<float>(m_FrameTime, 0.0333f);
	}
}

