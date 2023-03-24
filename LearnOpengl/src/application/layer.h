#pragma once
#include <glm/glm.hpp>
#include "vendor/imGui/imgui.h"

namespace Kenshin
{

	struct updatePayload
	{
		glm::vec2 viewport;
		float deltaTime;
		ImGuiIO io;
		bool isHover;
		bool isfocus;
		float ts;
	};

	class Layer
	{
	public:
		virtual ~Layer() = default;
		virtual void onAttach() { };
		virtual void onDetach() { };
		virtual void onUpdate(const updatePayload& payload) { };
		virtual void onUIRender(float fps, float deltaTime) { };
	};
}