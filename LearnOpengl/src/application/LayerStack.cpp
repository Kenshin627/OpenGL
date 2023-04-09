#include "kspch.h"
#include "LayerStack.h"

Kenshin::LayerStack::LayerStack()
{
	m_LayerInsert = m_Layers.begin();
}

Kenshin::LayerStack::~LayerStack()
{
	for (auto& layer : m_Layers)
	{
		delete layer;
	}
}

void Kenshin::LayerStack::PushLayer(Layer* layer)
{
	m_LayerInsert = m_Layers.emplace(m_LayerInsert, layer);
}

void Kenshin::LayerStack::PopLayer(Layer* layer)
{
	auto it = std::find(m_Layers.cbegin(), m_Layers.cend(), layer);
	if (it != m_Layers.cend())
	{
		m_Layers.erase(it);
		m_LayerInsert--;
	}
}

void Kenshin::LayerStack::PushOverLay(Layer* overLay)
{
	m_Layers.emplace_back(overLay);
}

void Kenshin::LayerStack::PopOverLay(Layer* overLay)
{
	auto it = std::find(m_Layers.cbegin(), m_Layers.cend(), overLay);
	if (it != m_Layers.cend())
	{
		m_Layers.erase(it);
	}
}
