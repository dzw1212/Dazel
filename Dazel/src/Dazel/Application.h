#pragma once
#include "Core.h"
#include "Events/ApplicationEvent.h"
#include "Window.h"
#include "LayerStack.h"
#include "GLFW/glfw3.h"
#include "glad/glad.h"


namespace DAZEL
{

	class DAZEL_API Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();

		void OnEvent(Event& event);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);
	private:
		bool OnWindowClose(WindowCloseEvent& event);

		std::unique_ptr<Window> m_Window;
		bool m_bRunning = true;
		LayerStack m_LayerStack;
	};

	Application* CreateApplication();
}


