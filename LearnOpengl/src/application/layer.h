#pragma once
#include <glm/glm.hpp>

namespace Kenshin
{
	class Layer
	{
	public:
		virtual ~Layer() = default;
		virtual void onAttach() { };
		virtual void onDetach() { };
		virtual void onUpdate(const glm::vec2& viewport, double ts) { };
		virtual void onUIRender() { };
	};
}