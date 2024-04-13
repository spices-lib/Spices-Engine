#pragma once
#include "Core/Core.h"

#include <functional>
#include <sstream>

namespace Spiecs {

	enum class EventType
	{
		None = 0,
		WindowClose, WindowResize, WindowFocus, WindowLostFocus, WindowMoved,
		AppTick, AppUpdate, AppRender,
		KeyPressed, KeyReleased, KeyTyped,
		MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled
	};

	enum EventCategory
	{
		None                      =  0,
		EventCategoryApplication  =  (1 << 0),
		EventCategoryInput        =  (1 << 1),
		EventCategoryKeyboard     =  (1 << 2),
		EventCategoryMouse        =  (1 << 3),
		EventCategoryMouseButton  =  (1 << 4)
	};

#define EVENT_CLASS_TYPE(type) static EventType GetStaticType() { return EventType::##type; }\
								virtual EventType GetEventType() const override { return GetStaticType(); }\
								virtual const char* GetName() const override { return #type; }

#define EVENT_CLASS_CATEGORY(category) virtual int GetCategoryFlags() const override { return category; }

#define BIND_EVENT_FN(x) std::bind(&x, this, std::placeholders::_1)

	class Event
	{
	public:
		using EventCallbackFn = std::function<void(Event&)>;
		friend class EventDispatcher;
	
		virtual EventType GetEventType() const = 0;
		virtual const char* GetName() const = 0;
		virtual int GetCategoryFlags() const = 0;
		virtual std::string ToString() const { return GetName(); }

		inline bool IsInCategory(EventCategory category)
		{
			return GetCategoryFlags() & category;
		}

		static EventCallbackFn GetEventCallbackFn();
		static void SetEventCallbackFn(const EventCallbackFn& callback);

		bool Handled = false;
	};


	class EventDispatcher
	{
		template<typename T>
		using EventFn = std::function<bool(T&)>;

	public:
		EventDispatcher(Event& event)
			:m_Event(event)
		{
		}

		template<typename T>
		bool Dispatch(EventFn<T> func)
		{
			if (m_Event.GetEventType() == T::GetStaticType())
			{
				m_Event.Handled = func(*(T*)&m_Event);
				return true;
			}
			return false;
		}
	private:
		Event& m_Event;
	};

	inline std::ostream& operator<<(std::ostream& os, const Event& e)
	{
		return os << e.ToString();
	}
}