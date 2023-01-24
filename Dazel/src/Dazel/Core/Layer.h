#pragma once

#include "Dazel/Core/Core.h"
#include "Dazel/Events/Event.h"

#include "Dazel/Core/Timestep.h"

namespace DAZEL
{
	class DAZEL_API Layer
	{
	public:
		Layer(const std::string& strName = "Layer");
		virtual ~Layer();

		virtual void OnAttach() {}
		virtual void OnDetach() {}
		virtual void OnUpdate(Timestep timeStep) {}
		virtual void OnEvent(Event& event) {}
		virtual void OnImGuiRender() {}

		inline const std::string& GetName() { return m_strDebugName; }
	protected:
		std::string m_strDebugName;
	};
}