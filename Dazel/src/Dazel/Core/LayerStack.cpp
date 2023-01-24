#include "DazelPCH.h"
#include "LayerStack.h"

namespace DAZEL
{
	LayerStack::LayerStack()
	{
	}

	LayerStack::~LayerStack()
	{
		for (auto layer : m_vecLayers)
			delete layer;
	}

	void LayerStack::PushLayer(Layer* layer)
	{
		m_vecLayers.emplace(m_vecLayers.begin() + m_LayerInsertIdx, layer);
		m_LayerInsertIdx++;
	}

	void LayerStack::PushOverlay(Layer* layer)
	{
		m_vecLayers.emplace_back(layer);
	}

	void LayerStack::PopLayer(Layer* layer)
	{
		auto it = std::find(m_vecLayers.begin(), m_vecLayers.end(), layer);
		if (it != m_vecLayers.end())
		{
			m_vecLayers.erase(it);
			m_LayerInsertIdx--;
		}
	}

	void LayerStack::PopOverlay(Layer* layer)
	{
		auto it = std::find(m_vecLayers.begin(), m_vecLayers.end(), layer);
		if (it != m_vecLayers.end())
		{
			m_vecLayers.erase(it);
		}
	}
}