#pragma once

#include "Event.h"

namespace DAZEL
{
	class DAZEL_API MouseMovedEvent : public Event
	{
	public:
		EVENT_CLASS_TYPE(MouseMoved)
			EVENT_CLASS_CATEGORY(eEventCategoryMouse | eEventCategoryInput)
	public:
		MouseMovedEvent(float fX, float fY)
			: m_fMouseX(fX), m_fMouseY(fY) {}
		inline float GetX() const { return m_fMouseX; }
		inline float GetY() const { return m_fMouseY; }

		std::string ToString() const override
		{
			return std::format("[MouseMovedEvent] MouseX:{} MouseY:{}", m_fMouseX, m_fMouseY);
		}

	private:
		float m_fMouseX;
		float m_fMouseY;
	};

	class DAZEL_API MouseScrolledEvent : public Event
	{
	public:
		EVENT_CLASS_TYPE(MouseScrolled)
			EVENT_CLASS_CATEGORY(eEventCategoryMouse | eEventCategoryInput)
	public:
		MouseScrolledEvent(float fX, float fY)
			: m_fOffsetX(fX), m_fOffsetY(fY) {}
		inline float GetOffsetX() { return m_fOffsetX; }
		inline float GetOffsetY() { return m_fOffsetY; }

		std::string ToString() const override
		{
			return std::format("[MouseScrolledEvent] OffsetX:{} OffsetY:{}", m_fOffsetX, m_fOffsetY);
		}
	private:
		float m_fOffsetX;
		float m_fOffsetY;
	};

	class DAZEL_API MouseButtonEvent : public Event
	{
	public:
		inline int GetMouseButton() const { return m_nButton; }
		EVENT_CLASS_CATEGORY(eEventCategoryMouseButton | eEventCategoryInput)
	protected:
		MouseButtonEvent(int nButton)
			: m_nButton(nButton) {}
		int m_nButton;
	};

	class DAZEL_API MouseButtonPressedEvent : public MouseButtonEvent
	{
	public:
		EVENT_CLASS_TYPE(MouseButtonPressed)
	public:
		MouseButtonPressedEvent(int nButton)
			: MouseButtonEvent(nButton) {}
		std::string ToString() const override
		{
			return std::format("[MouseButtonPressedEvent] Button:{}", m_nButton);
		}
	};

	class DAZEL_API MouseButtonReleasedEvent : public MouseButtonEvent
	{
	public:
		EVENT_CLASS_TYPE(MouseButtonReleased)
	public:
		MouseButtonReleasedEvent(int nButton)
			: MouseButtonEvent(nButton) {}
		std::string ToString() const override
		{
			return std::format("[MouseButtonReleasedEvent] Button:{}", m_nButton);
		}
	};
}