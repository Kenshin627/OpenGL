#pragma once
#include <string>
#include <vector>
#include <memory>
#include <functional>
#include "layer.h"

#include "vendor/imGui/imgui.h"

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
		static Application& Get();

		void run();
		void setMenuCallback(const std::function<void()>& menubarCallback) { m_MenubarCallback = menubarCallback; };
		template <typename T>
		void pushLayer()
		{
			static_assert(std::is_base_of<Layer, T>::value, "Pushed type is not subclass of layer!");
			auto layer = std::make_shared<T>();
			layer->onAttach();
			m_LayerSatack.emplace_back(layer);
		}

		void pushLayer(const std::shared_ptr<Layer>& layer)
		{
			m_LayerSatack.emplace_back(layer);
			layer->onAttach();
		}

		void close();
		float getTime() const;
		float getFPS() const;
		float getFrameTime() const;
		GLFWwindow* getWindowHandle() const { return m_windowHandle; };
	private:
		void init();
		void shutdown();
		void setTime();
		void framebuffer_size_callback(int width, int height);
		void mouseMove_callback(double xpos, double ypos);
		void mouseClick_callback(int button, int action);
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
		std::vector<std::shared_ptr<Layer>> m_LayerSatack;
	};

	Application* createApplication(int argc, char** argv);
}