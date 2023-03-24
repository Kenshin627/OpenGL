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
	Application::Application(const ApplicationSpecification& specification) :m_Specification(specification), m_MenubarCallback(nullptr), m_Running(false), m_TimeStep(0.0f), m_FrameTime(0.0f), m_LastFrameTime(0.0f), m_windowHandle(nullptr), isHoldRightBtn(false), isFirst(true), last_mouseX(0.0f), last_mouseY(0.0f), m_viewportSize(glm::vec2(0.0f, 0.0f))
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
		glfwInit();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		m_windowHandle = glfwCreateWindow(m_Specification.width, m_Specification.height, m_Specification.name.c_str(), nullptr, nullptr);
		glfwSetWindowUserPointer(m_windowHandle, this);
		
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

		//imGui
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
		//io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
		
		ImGuiStyle* style = &ImGui::GetStyle();
		ImGui::StyleColorsDark();
		io.Fonts->AddFontFromFileTTF("./resource/fonts/JetBrainsMono-Medium.ttf", 18, NULL, io.Fonts->GetGlyphRangesChineseFull());
		style->WindowPadding = ImVec2(3, 5);
		style->FramePadding  = ImVec2(4, 0);
		style->CellPadding   = ImVec2(3, 2);
		style->ItemSpacing = ImVec2(5, 4);
		style->ItemInnerSpacing = ImVec2(2, 3);
		style->ScrollbarSize = 1.0f;
		style->IndentSpacing = 25.0f;
		style->GrabMinSize = 5.0f;
			 
		style->WindowRounding = 0.0f;
		style->ChildRounding = 1.0f;
		style->FrameRounding = 1.0f;
		style->ScrollbarRounding = 5.0f;
		style->GrabRounding = 1.0f;
		style->TabRounding = 1.0f;
		
		ImVec4* colors = style->Colors;
		colors[ImGuiCol_Text] = ImVec4(1.00f, 0.68f, 0.17f, 1.00f);
		colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
		colors[ImGuiCol_WindowBg] = ImVec4(0.06f, 0.06f, 0.06f, 0.94f);
		colors[ImGuiCol_ChildBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
		colors[ImGuiCol_PopupBg] = ImVec4(0.08f, 0.08f, 0.08f, 0.94f);
		colors[ImGuiCol_Border] = ImVec4(0.43f, 0.43f, 0.50f, 0.50f);
		colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
		colors[ImGuiCol_FrameBg] = ImVec4(0.51f, 0.54f, 0.58f, 0.54f);
		colors[ImGuiCol_FrameBgHovered] = ImVec4(1.00f, 1.00f, 1.00f, 0.40f);
		colors[ImGuiCol_FrameBgActive] = ImVec4(0.26f, 0.59f, 0.98f, 0.67f);
		colors[ImGuiCol_TitleBg] = ImVec4(0.04f, 0.04f, 0.04f, 1.00f);
		colors[ImGuiCol_TitleBgActive] = ImVec4(0.08f, 0.08f, 0.08f, 1.00f);
		colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.00f, 0.00f, 0.00f, 0.51f);
		colors[ImGuiCol_MenuBarBg] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
		colors[ImGuiCol_ScrollbarBg] = ImVec4(0.02f, 0.02f, 0.02f, 0.53f);
		colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.31f, 0.31f, 0.31f, 1.00f);
		colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
		colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.51f, 0.51f, 0.51f, 1.00f);
		colors[ImGuiCol_CheckMark] = ImVec4(1.00f, 0.38f, 0.00f, 1.00f);
		colors[ImGuiCol_SliderGrab] = ImVec4(1.00f, 0.38f, 0.00f, 1.00f);
		colors[ImGuiCol_SliderGrabActive] = ImVec4(1.00f, 0.61f, 0.13f, 1.00f);
		colors[ImGuiCol_Button] = ImVec4(0.20f, 0.22f, 0.32f, 0.40f);
		colors[ImGuiCol_ButtonHovered] = ImVec4(1.00f, 0.38f, 0.00f, 1.00f);
		colors[ImGuiCol_ButtonActive] = ImVec4(1.00f, 0.38f, 0.00f, 1.00f);
		colors[ImGuiCol_Header] = ImVec4(0.63f, 0.80f, 0.97f, 0.31f);
		colors[ImGuiCol_HeaderHovered] = ImVec4(0.47f, 0.47f, 0.47f, 0.80f);
		colors[ImGuiCol_HeaderActive] = ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
		colors[ImGuiCol_Separator] = ImVec4(0.43f, 0.43f, 0.50f, 0.50f);
		colors[ImGuiCol_SeparatorHovered] = ImVec4(1.00f, 0.38f, 0.00f, 1.00f);
		colors[ImGuiCol_SeparatorActive] = ImVec4(1.00f, 0.38f, 0.00f, 1.00f);
		colors[ImGuiCol_ResizeGrip] = ImVec4(0.26f, 0.59f, 0.98f, 0.20f);
		colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.67f);
		colors[ImGuiCol_ResizeGripActive] = ImVec4(0.26f, 0.59f, 0.98f, 0.95f);
		colors[ImGuiCol_Tab] = ImVec4(0.66f, 0.66f, 0.66f, 0.86f);
		colors[ImGuiCol_TabHovered] = ImVec4(0.70f, 0.69f, 0.67f, 1.00f);
		colors[ImGuiCol_TabActive] = ImVec4(0.16f, 0.16f, 0.16f, 1.00f);
		colors[ImGuiCol_TabUnfocused] = ImVec4(0.07f, 0.10f, 0.15f, 0.97f);
		colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
		colors[ImGuiCol_DockingPreview] = ImVec4(1.00f, 0.38f, 0.00f, 1.00f);
		colors[ImGuiCol_DockingEmptyBg] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
		colors[ImGuiCol_PlotLines] = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
		colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
		colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
		colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
		colors[ImGuiCol_TableHeaderBg] = ImVec4(0.19f, 0.19f, 0.20f, 1.00f);
		colors[ImGuiCol_TableBorderStrong] = ImVec4(0.31f, 0.31f, 0.35f, 1.00f);
		colors[ImGuiCol_TableBorderLight] = ImVec4(0.23f, 0.23f, 0.25f, 1.00f);
		colors[ImGuiCol_TableRowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
		colors[ImGuiCol_TableRowBgAlt] = ImVec4(1.00f, 1.00f, 1.00f, 0.06f);
		colors[ImGuiCol_TextSelectedBg] = ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
		colors[ImGuiCol_DragDropTarget] = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
		colors[ImGuiCol_NavHighlight] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
		colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
		colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
		colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);

		ImGui_ImplGlfw_InitForOpenGL(m_windowHandle, true);
		ImGui_ImplOpenGL3_Init("#version 460");
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
		ImGuiIO& io = ImGui::GetIO();

		while (!glfwWindowShouldClose(m_windowHandle) && m_Running)
		{
			glfwPollEvents();
			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();

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
				if (ImGui::BeginMenu("Options"))
				{
					ImGui::MenuItem("Fullscreen", NULL, &opt_fullscreen);
					ImGui::MenuItem("Padding", NULL, &opt_padding);
					ImGui::Separator();

					if (ImGui::MenuItem("Flag: NoSplit", "", (dockspace_flags & ImGuiDockNodeFlags_NoSplit) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_NoSplit; }
					if (ImGui::MenuItem("Flag: NoResize", "", (dockspace_flags & ImGuiDockNodeFlags_NoResize) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_NoResize; }
					if (ImGui::MenuItem("Flag: NoDockingInCentralNode", "", (dockspace_flags & ImGuiDockNodeFlags_NoDockingInCentralNode) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_NoDockingInCentralNode; }
					if (ImGui::MenuItem("Flag: AutoHideTabBar", "", (dockspace_flags & ImGuiDockNodeFlags_AutoHideTabBar) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_AutoHideTabBar; }
					if (ImGui::MenuItem("Flag: PassthruCentralNode", "", (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode) != 0, opt_fullscreen)) { dockspace_flags ^= ImGuiDockNodeFlags_PassthruCentralNode; }
					ImGui::Separator();
					ImGui::EndMenu();
				}

				ImGui::EndMenuBar();
			}

			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
			ImGui::Begin("Scene");
			ImGuiIO io = ImGui::GetIO();
			ImVec2 size = ImGui::GetContentRegionAvail();			
			
			for (auto& layer : m_LayerSatack)
			{
				layer->onUpdate(Kenshin::updatePayload{ glm::vec2(size.x, size.y), m_FrameTime, io, ImGui::IsWindowHovered(), ImGui::IsWindowFocused(), getTime()});
			}
			ImGui::End();
			ImGui::PopStyleVar();

			for (auto& layer : m_LayerSatack)
			{
				layer->onUIRender(getFPS(), getFrameTime());
			}

			ImGui::End();
			ImGui::Render();

/*			int display_w, display_h;
			glfwGetFramebufferSize(m_windowHandle, &display_w, &display_h);	*/		
			
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());			
			glfwSwapBuffers(m_windowHandle);
			setTime();
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
		double currentTime = getTime();
		m_FrameTime = currentTime - m_LastFrameTime;
		m_LastFrameTime = currentTime;
		m_TimeStep = std::min<float>(m_FrameTime, 0.0333f);
	}
}

