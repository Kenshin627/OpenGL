#pragma once

namespace Kenshin
{
	class Layer
	{
	public:
		virtual ~Layer() = default;
		virtual void onAttach() { };
		virtual void onDetach() { };
		virtual void onUpdate(double ts) { };
		virtual void onUIRender() { };
	};
}