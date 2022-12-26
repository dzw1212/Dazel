#pragma once

#include "Dazel/Events/Event.h"
#include "Dazel/Core.h"

#include "Dazel/Events/ApplicationEvent.h"
#include "Dazel/Events/KeyEvent.h"
#include "Dazel/Events/MouseEvent.h"

namespace DAZEL
{
	struct sWindowProps
	{
		std::string strTitle;
		UINT uiWidth;
		UINT uiHeight;

		sWindowProps(const std::string strTitle = "Dazel Engine",
			UINT uiWidth = 1920,
			UINT uiHeight = 1080)
			: strTitle(strTitle), uiWidth(uiWidth), uiHeight(uiHeight) {}

		void Reset()
		{
			strTitle.clear();
			uiWidth = 0;
			uiHeight = 0;
		}
	};

	class DAZEL_API Window
	{
	public:
		using EventCallbackFn = std::function<void(Event&)>;

		virtual ~Window() {}
		virtual void OnUpdate() = 0;

		virtual UINT GetWidth() const = 0;
		virtual UINT GetHeight() const = 0;

		virtual void SetEventCallback(const EventCallbackFn& callback) = 0;
		virtual void SetVSync(bool bEnable) = 0;
		virtual bool IsVSync() const = 0;
		static Window* Create(const sWindowProps& props = sWindowProps());
	};
}