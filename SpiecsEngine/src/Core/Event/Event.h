/**
* @file Event.h.
* @brief The Event Class Definitions.
* @author The Cherno.
*/

#pragma once
#include "Core/Core.h"

#include <functional>
#include <sstream>

namespace Spiecs {
	
	/**
	* @brief This enum defines what specific event type is.
	*/
	enum class EventType
	{
		/**
		* @brief None Event.
		*/
		None = 0,

		/**
		* @brief Window Event.
		*/
		WindowClose, WindowResize, WindowResizeOver, WindowFocus, WindowLostFocus, WindowMoved,

		/**
		* @brief Application Event.
		*/
		AppTick, AppUpdate, AppRender,

		/**
		* @brief Slate Event.
		*/
		SlateResize,

		/**
		* @brief Key Input Event.
		*/
		KeyPressed, KeyReleased, KeyTyped,

		/**
		* @brief Mouse Input Event.
		*/
		MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled,

		/**
		* @brief World Event.
		*/
		MeshAdded,
	};

	/**
	* @brief This enum defines what specific event category is.
	*/
	enum EventCategory
	{
		None                      =  0,           /* @brief None */
		EventCategoryApplication  =  (1 << 0),    /* @brief Application */
		EventCategorySlate        =  (1 << 1),    /* @brief Slate */
		EventCategoryInput        =  (1 << 2),    /* @brief Input */
		EventCategoryKeyboard     =  (1 << 3),    /* @brief Keyboard */
		EventCategoryMouse        =  (1 << 4),    /* @brief Mouse */
		EventCategoryMouseButton  =  (1 << 5),    /* @brief MouseButton */
		EventCategoryWorld         = (1 << 6),    /* @brief World */
	};

/**
* @brief Defines Event type.
*/
#define EVENT_CLASS_TYPE(type)                                                  \
	static EventType GetStaticType() { return EventType::##type; }              \
	virtual EventType GetEventType() const override { return GetStaticType(); } \
	virtual const char* GetName()    const override { return #type; }

/**
* @brief Defines Event category.
*/
#define EVENT_CLASS_CATEGORY(category)                                          \
	virtual int GetCategoryFlags() const override { return category; }
	
/**
* @brief Bind Event.
*/
#define BIND_EVENT_FN(x)                                                        \
	std::bind(&x, this, std::placeholders::_1)

	/**
	* @brief This Class is the basic Event Class.
	* Inherit from it and create specific event class.
	*/
	class Event
	{
	public:

		/**
		* @brief Destructor Function.
		*/
		virtual ~Event() = default;

		/**
		* @brief Helper type of Function pointer.
		*/
		using EventCallbackFn = std::function<void(Event&)>;

		/**
		* @brief Allow EventDispatcher access all data of this.
		*/
		friend class EventDispatcher;

		/**
		* @brief Event Information Function, must be implemented by EVENT_CLASS_TYPE and EVENT_CLASS_CATEGORY.
		*/
		virtual EventType GetEventType()    const = 0;
		virtual const char* GetName()       const = 0;
		virtual int GetCategoryFlags()      const = 0;
		virtual std::string ToString()      const { return GetName(); }

		/**
		* @brief Judgement if a given category is contained by this event class.
		* @param[in] category In category.
		* @return Returns true if given category is contained by this event class.
		*/
		inline bool IsInCategory(EventCategory category)
		{
			return GetCategoryFlags() & category;
		}

		/**
		* @brief Get Global Root Event Function Pointer.
		* @return Returns the Global Root Event Function Pointer.
		*/
		static EventCallbackFn GetEventCallbackFn();

		/**
		* @brief Set Global Root Event Function Pointer.
		* @param[in] callback The Global Root Event Function Pointer.
		* @note Usually call it once in whole Application lifetime.
		*/
		static void SetEventCallbackFn(const EventCallbackFn& callback);

		/**
		* @brief True if this event is handled.
		*/
		bool Handled = false;
	};

	/**
	* @brief This Class store a Specific Event type first
	* and Dispatch a event handle function to it.
	*/
	class EventDispatcher
	{

		/**
		* @brief Helper type of the function pointer to handle specific event.
		* @param[in] T Specific Event Class.
		* @return Returns true if needs block event chain.
		*/
		template<typename T>
		using EventFn = std::function<bool(T&)>;

	public:

		/**
		* @brief Constructor Function.
		* @param[in] event Specific Event reference.
		*/
		EventDispatcher(Event& event)
			:m_Event(event)
		{}

		/**
		* @brief Dispatch the specific Event handle function pointer to Event Class.
		* @param[in] EventFn<T> Specific Event handle function.
		* @return Returns true if execute function pointer.
		*/
		template<typename T>
		bool Dispatch(EventFn<T> func)
		{

			/**
			* @brief If Specific Event handle function takes the same type with Stored Event type,
			* will execute the function pointer.
			*/
			if (m_Event.GetEventType() == T::GetStaticType())
			{
				m_Event.Handled = func(*(T*)&m_Event);
				
				return true;
			}
			
			return false;
		}
		
	private:

		/**
		* @brief Store the specific Event reference.
		*/
		Event& m_Event;
	};

	/**
	* @brief Serialize Event Type.
	*/
	inline std::ostream& operator<<(std::ostream& os, const Event& e)
	{
		return os << e.ToString();
	}
}