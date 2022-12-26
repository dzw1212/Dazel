#pragma once

#include "Dazel/Layer.h"

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

		void OnAttach();
		void OnDetach();
		void OnUpdate();
		void OnEvent(Event& event);
	public:
		bool OnMouseButtonPressedEvent(MouseButtonPressedEvent& event);
		bool OnMouseButtonReleasedEvent(MouseButtonReleasedEvent& event);
		bool OnMouseMovedEvent(MouseMovedEvent& event);
		bool OnMouseScrolledEvent(MouseScrolledEvent& event);
		bool OnKeyPressedEvent(KeyPressedEvent& event);
		bool OnKeyReleasedEvent(KeyReleasedEvent& event);
		bool OnWindowResizeEvent(WindowResizeEvent& event);
		bool OnKeyTypedEvent(KeyTypedEvent& event);
	};
}