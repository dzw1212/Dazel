#pragma once

#include "Dazel/Core.h"
#include "Dazel/Events/Event.h"

namespace DAZEL
{
	class DAZEL_API Layer
	{
	public:
		Layer(const std::string& strName = "Layer");
		virtual ~Layer();

		virtual void OnAttach() {}
		virtual void OnDetach() {}
		virtual void OnUpdate() {}
		virtual void OnEvent(Event& event) {}

		inline const std::string& GetName() { return m_strDebugName; }
	protected:
		std::string m_strDebugName;
	};
}