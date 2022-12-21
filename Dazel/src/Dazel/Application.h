#pragma once
#include "Core.h"
#include "Events/ApplicationEvent.h"
#include "Window.h"
#include "GLFW/glfw3.h"


namespace DAZEL
{

	class DAZEL_API Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();
	private:
		std::unique_ptr<Window> m_Window;
		bool m_bRunning = true;
	};

	Application* CreateApplication();
}


