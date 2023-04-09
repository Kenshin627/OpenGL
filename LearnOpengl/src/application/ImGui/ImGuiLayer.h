#pragma once
#include "../layer.h"

namespace Kenshin
{
	class ImGuiLayer:public Layer
	{
	public:
		ImGuiLayer() :Layer("ImGuiLayer") {}
		virtual void onAttach() override;
		virtual void onDetach() override;
		virtual void onUpdate(const updatePayload& payload) override;
	};
}