#pragma once

#include "Dazel/Window.h"
#include "GLFW/glfw3.h"

namespace DAZEL
{
	class WindowsWindow : public Window
	{
	public:
		WindowsWindow(const sWindowProps& props);
		virtual ~WindowsWindow();

		void OnUpdate() override;

		inline UINT GetWidth() const override { return m_Data.sProps.uiWidth; }
		inline UINT GetHeight() const override { return m_Data.sProps.uiHeight; }

		inline void SetEventCallback(const EventCallbackFn& callback) override
		{
			m_Data.EventCallback = callback;
		}
		void SetVSync(bool bEnable) override;
		bool IsVSync() const override;
	private:
		virtual void Init(const sWindowProps& props);
		virtual void Shutdown();
	private:
		GLFWwindow* m_Window;

		struct sWindowData
		{
			sWindowProps sProps;
			bool bVSync;
			EventCallbackFn EventCallback;
		};
		sWindowData m_Data;
	};
}