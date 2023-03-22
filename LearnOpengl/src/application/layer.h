#pragma once
#include <glm/glm.hpp>
#include "vendor/imGui/imgui.h"

namespace Kenshin
{
	class Layer
	{
	public:
		virtual ~Layer() = default;
		virtual void onAttach() { };
		virtual void onDetach() { };
		virtual void onUpdate(const glm::vec2& viewport, float deltaTime, const ImGuiIO& io) { };
		virtual void onUIRender(float fps, float deltaTime) { };
	};
}