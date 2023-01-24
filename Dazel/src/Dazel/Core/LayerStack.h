#pragma once

#include "Dazel/Core/Core.h"
#include "Layer.h"

namespace DAZEL
{
	class DAZEL_API LayerStack
	{
	public:
		using LayerStackIter = std::vector<Layer*>::iterator;
		using LayerStackRevIter = std::vector<Layer*>::reverse_iterator;
		LayerStack();
		~LayerStack();

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);
		void PopLayer(Layer* layer);
		void PopOverlay(Layer* overlay);

		LayerStackIter begin() { return m_vecLayers.begin(); }
		LayerStackIter end() { return m_vecLayers.end(); }
		LayerStackRevIter rbegin() { return m_vecLayers.rbegin(); }
		LayerStackRevIter rend() { return m_vecLayers.rend(); }
	
	private:
		std::vector<Layer*> m_vecLayers;
		UINT m_LayerInsertIdx = 0;
	};
}