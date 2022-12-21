#pragma once

#include "Event.h"

namespace DAZEL
{
	class DAZEL_API WindowResizeEvent : public Event
	{
	public:
		EVENT_CLASS_CATEGORY(eEventCategoryApplication)
		EVENT_CLASS_TYPE(WindowResize)
	public:
		WindowResizeEvent(UINT uiWidth, UINT uiHeight)
			: m_uiWidth(uiWidth), m_uiHeight(uiHeight) {}
		inline UINT GetWidth() const { return m_uiWidth; }
		inline UINT GetHeight() const { return m_uiHeight; }

		std::string ToString() const override
		{
			return std::format("[WindowResizeEvent] width:{} height:{}", m_uiWidth, m_uiHeight);
		}

	private:
		UINT m_uiWidth;
		UINT m_uiHeight;
	};

	class DAZEL_API WindowCloseEvent : public Event
	{
	public:
		EVENT_CLASS_TYPE(WindowClose)
		EVENT_CLASS_CATEGORY(eEventCategoryApplication)
	public:
		WindowCloseEvent() {}
	};

	class DAZEL_API AppTickEvent : public Event
	{
	public:
		EVENT_CLASS_TYPE(AppTick)
		EVENT_CLASS_CATEGORY(eEventCategoryApplication)
	public:
		AppTickEvent() {}
	};

	class DAZEL_API AppUpdateEvent : public Event
	{
	public:
		EVENT_CLASS_TYPE(AppUpdate)
		EVENT_CLASS_CATEGORY(eEventCategoryApplication)
	public:
		AppUpdateEvent() {}
	};

	class DAZEL_API AppRenderEvent : public Event
	{
	public:
		EVENT_CLASS_TYPE(AppRender)
		EVENT_CLASS_CATEGORY(eEventCategoryApplication)
	public:
		AppRenderEvent() {}
	};
}