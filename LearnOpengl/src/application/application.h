#pragma once
#include "vendor/imGui/imgui.h"
#include "LayerStack.h"
#include "application/ImGui/ImGuiLayer.h"

struct GLFWwindow;

namespace Kenshin
{
	struct ApplicationSpecification
	{
		std::string name = "KenshinApp";
		uint32_t width	 = 1600;
		uint32_t height  = 900;
	};
	class Application
	{
	public:
		Application(const ApplicationSpecification& spec = ApplicationSpecification());
		~Application();
		static Application& Get() { return *s_Instance; };

		void run();
		void setMenuCallback(const std::function<void()>& menubarCallback) { m_MenubarCallback = menubarCallback; };
		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overLay);

		void close();
		float getTime() const;
		float getFPS() const;
		float getFrameTime() const;
		GLFWwindow* getWindowHandle() const { return m_windowHandle; };
	private:
		void init();
		void shutdown();
		void setTime();
	private:
		ApplicationSpecification m_Specification;
		std::function<void()> m_MenubarCallback;
		bool m_Running = false;
		float m_TimeStep = 0.0f;
		float m_FrameTime = 0.0f;
		float m_LastFrameTime = 0.0f;
		GLFWwindow* m_windowHandle = nullptr;
		bool isHoldRightBtn;
		bool isFirst;
		float last_mouseX;
		float last_mouseY;
		LayerStack m_LayerStack;
		glm::vec2 m_viewportSize;
		static Application* s_Instance;
		ImGuiLayer uiLayer;
	};

	Application* createApplication(int argc, char** argv);
}