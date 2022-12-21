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
			glfwSwapInterval(1); //������ֱͬ��,����Ϊ60HZ
		else
			glfwSwapInterval(0); //�رմ�ֱͬ��
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
		glfwSetWindowUserPointer(m_Window, &m_Data); //���Զ������ݴ��ݸ�window,����ͨ��glfwGetWindowUserPointer��ȡ
		SetVSync(true);
	}

	void DAZEL::WindowsWindow::Shutdown()
	{
	}
}
