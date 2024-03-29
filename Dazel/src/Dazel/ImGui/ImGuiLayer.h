#pragma once

#include "Dazel/Core/Layer.h"

#include "Dazel/Events/ApplicationEvent.h"
#include "Dazel/Events/KeyEvent.h"
#include "Dazel/Events/MouseEvent.h"

namespace DAZEL
{
	class DAZEL_API ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer();

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnUpdate(Timestep timeStep) override;
		virtual void OnEvent(Event& event) override;
		virtual void OnImGuiRender() override;
	public:
		void Begin();
		void End();

		void BlockEvents(bool bBlock) { m_bBlockEvents = bBlock; }
	private:
		float m_fTime = 0.f;
		bool m_bBlockEvents = false;
	};
}