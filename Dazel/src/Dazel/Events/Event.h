#pragma once
#include "DazelPCH.h"
#include "Dazel/Core/Core.h"

namespace DAZEL
{
	//currently blocking mode
	//todo:
	//    buffer events

	enum class EventType
	{
		None = 0,
		
		WindowClose,
		WindowResize,
		WindowFocus,
		WindowLostFocus,
		WindowMoved,

		AppTick,
		AppUpdate,
		AppRender,

		KeyPressed,
		KeyReleased,
		KeyTyped,

		MouseButtonPressed,
		MouseButtonReleased,

		MouseMoved,
		MouseScrolled,
	};

	enum EventCategory
	{
		eNone = 0,
		eEventCategoryApplication	= BIT(0),
		eEventCategoryInput			= BIT(1),
		eEventCategoryKeyboard		= BIT(2),
		eEventCategoryMouse			= BIT(3),
		eEventCategoryMouseButton	= BIT(4),
	};

#define EVENT_CLASS_TYPE(type)\
	static EventType GetStaticType() { return EventType::##type; }\
	virtual EventType GetEventType() const override { return GetStaticType(); }\
	virtual const char* GetName() const override { return #type; }

#define EVENT_CLASS_CATEGORY(category)\
	virtual int GetCategoryFlags() const override { return category; }

	class DAZEL_API Event
	{
		friend class EventDispatcher;
	public:
		//���´��麯�����涨Event���������ʵ��
		virtual EventType GetEventType() const = 0;
		virtual const char* GetName() const = 0;
		virtual int GetCategoryFlags() const = 0;

		//�������дToString��������Ĭ�Ϸ���Name
		virtual std::string ToString() const { return GetName(); }

		inline bool IsInCategory(EventCategory category) { return GetCategoryFlags() & category; }

		inline bool bIsHandled() { return m_bHandled; }

		bool m_bHandled = false; //�Ƿ��ѱ�����
	};

	class EventDispatcher
	{
		template<typename T>
		using EventFn = std::function<bool(T&)>;
	public:
		EventDispatcher(Event& event)
			: m_Event(event){}

		template<typename T>
		bool Dispatch(EventFn<T> func)
		{
			if (m_Event.GetEventType() == T::GetStaticType())
			{
				m_Event.m_bHandled = func(*(T*)&m_Event);
				return true;
			}
			return false;
		}

	private:
		Event& m_Event;
	};

	inline std::ostream& operator<<(std::ostream& os, const Event& event)
	{
		return os << event.ToString();
	};
}