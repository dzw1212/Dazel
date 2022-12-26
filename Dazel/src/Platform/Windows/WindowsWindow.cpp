#include "DazelPCH.h"
#include "WindowsWindow.h"

#include "GLFW/glfw3.h"
#include "glad/glad.h"

namespace DAZEL
{
	static bool s_bGLFWInitialized = false;

	static void GLFWErrCallBack(int nErrCode, const char* pcErrDesc)
	{
		CORE_LOG_ERROR("[GLFW Error] ErrCode:{} ErrDesc:{}", nErrCode, pcErrDesc);
	}

	Window* Window::Create(const sWindowProps& props)
	{
		return new WindowsWindow(props);
	}

	WindowsWindow::WindowsWindow(const sWindowProps& props)
	{
		Init(props);
	}

	WindowsWindow::~WindowsWindow()
	{
		Shutdown();
	}

	void WindowsWindow::OnUpdate()
	{
		glfwPollEvents();
		glfwSwapBuffers(m_Window);
	}

	void WindowsWindow::SetVSync(bool bEnable)
	{
		if (bEnable)
			glfwSwapInterval(1); //开启垂直同步,并设为60HZ
		else
			glfwSwapInterval(0); //关闭垂直同步
		m_Data.bVSync = bEnable;
	}

	bool WindowsWindow::IsVSync() const
	{
		return m_Data.bVSync;
	}

	void WindowsWindow::Init(const sWindowProps& props)
	{
		m_Data.sProps = props;
		LOG_INFO("[Creating Windows Window] Title:<{}> Size:({},{})", 
			m_Data.sProps.strTitle, m_Data.sProps.uiWidth, m_Data.sProps.uiHeight);

		if (!s_bGLFWInitialized)
		{
			int nRet = glfwInit();
			CORE_ASSERT(nRet == GLFW_TRUE, "GLFW Init failed");
			glfwSetErrorCallback(GLFWErrCallBack);
			s_bGLFWInitialized = true;
		}

		m_Window = glfwCreateWindow((int)props.uiWidth, (int)props.uiHeight, props.strTitle.c_str(), nullptr, nullptr);
		CORE_ASSERT(m_Window, "GLFW create window failed");
		glfwMakeContextCurrent(m_Window);
		glfwSetWindowUserPointer(m_Window, &m_Data); //将自定义数据传递给window,后续通过glfwGetWindowUserPointer获取
		SetVSync(true);

		int nRet = gladLoadGLLoader(GLADloadproc(glfwGetProcAddress));
		CORE_ASSERT(nRet, "GLAD Init failed");

		glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int nWidth, int nHeight)
			{
				sWindowData* pWindowData = (sWindowData*)glfwGetWindowUserPointer(window);
				CORE_ASSERT(pWindowData, "WindowData is NULL");
				WindowResizeEvent event(nWidth, nHeight);
				pWindowData->sProps.uiWidth = nWidth;
				pWindowData->sProps.uiHeight = nHeight;
				pWindowData->EventCallback(event);
			}
		);

		glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window)
			{
				sWindowData* pWindowData = (sWindowData*)glfwGetWindowUserPointer(window);
				CORE_ASSERT(pWindowData, "WindowData is NULL");
				WindowCloseEvent event;
				pWindowData->EventCallback(event);
			}
		);

		glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int nKey, int nScanCode, int nAction, int nMods)
			{
				sWindowData* pWindowData = (sWindowData*)glfwGetWindowUserPointer(window);
				CORE_ASSERT(pWindowData, "WindowData is NULL");

				switch (nAction)
				{
					case GLFW_PRESS:
					{
						KeyPressedEvent event(nKey, 0);
						pWindowData->EventCallback(event);
						break;
					}
					case GLFW_RELEASE:
					{
						KeyReleasedEvent event(nKey);
						pWindowData->EventCallback(event);
						break;
					}
					case GLFW_REPEAT:
					{
						KeyPressedEvent event(nKey, 1);
						pWindowData->EventCallback(event);
						break;
					}
					default:
					{
						CORE_LOG_WARN("Unknown Key Action");
						break;
					}
				}
			}
		);

		glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int nButton, int nAction, int nMods)
			{
				sWindowData* pWindowData = (sWindowData*)glfwGetWindowUserPointer(window);
				CORE_ASSERT(pWindowData, "WindowData is NULL");

				switch (nAction)
				{
					case GLFW_PRESS:
					{
						MouseButtonPressedEvent event(nButton);
						pWindowData->EventCallback(event);
						break;
					}
					case GLFW_RELEASE:
					{
						MouseButtonReleasedEvent event(nButton);
						pWindowData->EventCallback(event);
						break;
					}
					default:
					{
						CORE_LOG_WARN("Unknown Mouse Action");
						break;
					}
				}
			}
		);

		glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double dOffsetX, double dOffsetY)
			{
				sWindowData* pWindowData = (sWindowData*)glfwGetWindowUserPointer(window);
				CORE_ASSERT(pWindowData, "WindowData is NULL");

				MouseScrolledEvent event(dOffsetX, dOffsetY);
				pWindowData->EventCallback(event);
			}
		);

		glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double dPosX, double dPosY)
			{
				sWindowData* pWindowData = (sWindowData*)glfwGetWindowUserPointer(window);
				CORE_ASSERT(pWindowData, "WindowData is NULL");

				MouseMovedEvent event(dPosX, dPosY);
				pWindowData->EventCallback(event);
			}
		);

		glfwSetCharCallback(m_Window, [](GLFWwindow* window, UINT uiChar)
			{
				sWindowData* pWindowData = (sWindowData*)glfwGetWindowUserPointer(window);
				CORE_ASSERT(pWindowData, "WindowData is NULL");

				LOG_ERROR("uiChar = {}", uiChar);

				KeyTypedEvent event(uiChar);
				pWindowData->EventCallback(event);
			}
		);
	}

	void WindowsWindow::Shutdown()
	{
		glfwDestroyWindow(m_Window);
	}

}
