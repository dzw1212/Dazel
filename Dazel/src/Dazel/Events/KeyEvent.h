#pragma once

#include "Event.h"

namespace DAZEL
{
	class DAZEL_API KeyEvent : public Event
	{
	public:
		inline int GetKeyCode() const { return m_nKeyCode; }
		EVENT_CLASS_CATEGORY(eEventCategoryInput | eEventCategoryKeyboard);
	protected:
		//protected的构造函数，意味着只有子类才可以构造它
		KeyEvent(int nKeyCode)
			: m_nKeyCode(nKeyCode){}
		int m_nKeyCode;
	};

	class DAZEL_API KeyPressedEvent : public KeyEvent
	{
	public:
		EVENT_CLASS_TYPE(KeyPressed);
	public:
		KeyPressedEvent(int nKeyCode, int nRepeatCount)
			: KeyEvent(nKeyCode), m_nRepeatCount(nRepeatCount) {}

		inline int GetRepeatCount() const { return m_nRepeatCount; }

		std::string ToString() const override
		{
			return std::format("[KeyPressedEvent] KeyCode:{} RepeatCount:{}", m_nKeyCode, m_nRepeatCount);
		}
	protected:
		int m_nRepeatCount;
	};

	class DAZEL_API KeyReleasedEvent : public KeyEvent
	{
	public:
		EVENT_CLASS_TYPE(KeyReleased)
	public:
		KeyReleasedEvent(int nKeyCode)
			: KeyEvent(nKeyCode) {}

		std::string ToString() const override
		{
			return std::format("[KeyReleasedEvent] KeyCode:{}", m_nKeyCode);
		}
		
	};
}