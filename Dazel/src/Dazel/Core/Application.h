#pragma once
#include "Core.h"
#include "Dazel/Events/ApplicationEvent.h"
#include "Dazel/ImGui/ImGuiLayer.h"
#include "Window.h"
#include "LayerStack.h"

#include "Dazel/Renderer/Shader.h"
#include "Dazel/Renderer/Buffer.h"
#include "Dazel/Renderer/VertexArray.h"

#include "Dazel/Camera/OrthographicCamera.h"

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

		inline static Application& Get() { return *s_Inst; }
		inline Window& GetWindow() { return *m_Window; }
	private:
		bool OnWindowClose(WindowCloseEvent& event);
		bool OnWindowResize(WindowResizeEvent& event);

	private:
		Scope<Window> m_Window;
		ImGuiLayer* m_ImGuiLayer;
		bool m_bRunning = true;
		LayerStack m_LayerStack;
		static Application* s_Inst;
		float m_fLastFrameTime = 0.f;
		bool m_bMinimized = false;
	};

	Application* CreateApplication();
}


