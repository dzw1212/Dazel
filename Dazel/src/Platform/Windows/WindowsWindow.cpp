#include "DazelPCH.h"
#include "WindowsWindow.h"

namespace DAZEL
{
	static bool s_bGLFWInitialized = false;

	Window* Create(const sWindowProps& props)
	{
		return new WindowsWindow(props);
	}

	DAZEL::WindowsWindow::WindowsWindow(const sWindowProps& props)
	{
		Init(props);
	}

	DAZEL::WindowsWindow::~WindowsWindow()
	{
		Shutdown();
	}

	void DAZEL::WindowsWindow::OnUpdate()
	{
	}

	void DAZEL::WindowsWindow::SetVSync(bool bEnable)
	{
		if (bEnable)
			glfwSwapInterval(1); //开启垂直同步,并设为60HZ
		else
			glfwSwapInterval(0); //关闭垂直同步
		m_Data.bVSync = bEnable;
	}

	bool DAZEL::WindowsWindow::IsVSync() const
	{
		return m_Data.bVSync;
	}

	void DAZEL::WindowsWindow::Init(const sWindowProps& props)
	{
		m_Data.sProps = props;
		LOG_INFO("[Creating Windows Window] Title:{} Size:({},{})", 
			m_Data.sProps.strTitle, m_Data.sProps.uiWidth, m_Data.sProps.uiHeight);
		if (!s_bGLFWInitialized)
		{
			int nRet = glfwInit();
			CORE_ASSERT(nRet == GLFW_TRUE, "Init GLFW failed");
			s_bGLFWInitialized = true;
		}
		m_Window = glfwCreateWindow((int)props.uiWidth, (int)props.uiHeight, props.strTitle.c_str(), nullptr, nullptr);
		glfwMakeContextCurrent(m_Window);
		glfwSetWindowUserPointer(m_Window, &m_Data); //将自定义数据传递给window,后续通过glfwGetWindowUserPointer获取
		SetVSync(true);
	}

	void DAZEL::WindowsWindow::Shutdown()
	{
	}
}
