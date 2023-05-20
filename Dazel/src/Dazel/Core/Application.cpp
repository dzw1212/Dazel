#include "DazelPCH.h"
#include "Application.h"

#include "GLFW/glfw3.h"

#include "glad/glad.h"
#include "glm/glm.hpp"

#include "Dazel/Renderer/Shader.h"
#include "Dazel/Renderer/Buffer.h"

#include "Dazel/Renderer/RendererCommand.h"
#include "Dazel/Renderer/Renderer.h"

#include "Dazel/Scripting/ScriptEngine.h"

#include "Dazel/Core/Timestep.h"

namespace DAZEL
{
	Application* Application::s_Inst = nullptr;

	Application::Application(const std::string& strAppName, ApplicationCommandLineArgs args)
		: m_CommandLineArgs(args)
	{
		PROFILE_FUNCTION();

		CORE_ASSERT(s_Inst == nullptr, "Create Application failed. Already exist.");
		s_Inst = this;
		sWindowProps props;
		props.strTitle = strAppName;
		props.uiWidth = 2560;
		props.uiHeight = 1440;
		m_Window = Window::Create(props);
		m_Window->SetEventCallback(BIND_EVENT_FN(Application::OnEvent));

		Renderer::Init();
		ScriptEngine::Init();

		m_ImGuiLayer = new ImGuiLayer;
		PushOverlay(m_ImGuiLayer);
	}

	Application::~Application()
	{
		Renderer::Shutdown();
		ScriptEngine::Shutdown();
		s_Inst = nullptr;
	}

	void Application::OnEvent(Event& event)
	{
		PROFILE_FUNCTION();

		EventDispatcher dispatcher(event);
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(Application::OnWindowClose));
		dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FN(Application::OnWindowResize));

		for (auto it = m_LayerStack.rbegin(); it != m_LayerStack.rend(); ++it)
		{
			(*it)->OnEvent(event);
			if (event.bIsHandled())
				break;
		}
	}

	void Application::PushLayer(Layer* layer)
	{
		PROFILE_FUNCTION();

		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverlay(Layer* layer)
	{
		PROFILE_FUNCTION();

		m_LayerStack.PushOverlay(layer);
		layer->OnAttach();
	}

	void Application::SubmitFuncToMainThread(const std::function<void()>& function)
	{
		std::scoped_lock<std::mutex> lock(m_MainThreadFuncQueueMutex);
		m_vecMainThreadFuncQueue.push_back(function);
	}

	void Application::ExecuteMainThreadFuncQueue()
	{
		std::scoped_lock<std::mutex> lock(m_MainThreadFuncQueueMutex);

		if (m_vecMainThreadFuncQueue.size() > 0)
		{
			for (auto& func : m_vecMainThreadFuncQueue)
				func();

			m_vecMainThreadFuncQueue.clear();
		}
	}

	bool Application::OnWindowClose(WindowCloseEvent& event)
	{
		m_bRunning = false;
		return true;
	}

	bool Application::OnWindowResize(WindowResizeEvent& event)
	{
		if ((event.GetWidth() == 0) && (event.GetHeight()))
			m_bMinimized = true;
		else
			m_bMinimized = false;

		Renderer::SetViewport(0, 0, event.GetWidth(), event.GetHeight());

		return false;
	}

	void Application::Run()
	{
		PROFILE_FUNCTION();

		while (m_bRunning)
		{
			float fCurTime = glfwGetTime();
			Timestep timeStep = fCurTime - m_fLastFrameTime;
			m_fLastFrameTime = fCurTime;

			ExecuteMainThreadFuncQueue();

			if (!m_bMinimized)
			{
				{
					PROFILE_SCOPE("LayerStack Update");
					for (auto it : m_LayerStack)
					{
						it->OnUpdate(timeStep);
					}
				}
				
			}
			{
				PROFILE_SCOPE("LayerStack ImGuiRender");
				m_ImGuiLayer->Begin();
				for (auto it : m_LayerStack)
				{
					it->OnImGuiRender();
				}
				m_ImGuiLayer->End();
			}
			

			m_Window->OnUpdate();
		};
	}
}
