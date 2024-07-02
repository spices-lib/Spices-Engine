/**
* @file KeyEvent.h.
* @brief The KeyEvent Class Definitions.
* @author The Cherno.
*/

#pragma once
#include "Core/Core.h"
#include "Event.h"

namespace Spiecs {

	/**
	* @brief This Class is inherited from Event Class.
	* Inherit from it and create specific KeyEvent class. 
	*/
	class KeyEvent : public Event
	{
	public:

		/**
		* @brief Destructor Function.
		*/
		virtual ~KeyEvent() override = default;

		/**
		* @brief Get Key Input Code.
		* @return Returns the Key Input Code.
		*/
		inline int GetKeyCode() const { return m_KeyCode; }

		/**
		* @brief Specific this Class Category with EventCategoryKeyboard and EventCategoryInput.
		*/
		EVENT_CLASS_CATEGORY(EventCategoryKeyboard | EventCategoryInput)
		
	protected:

		/**
		* @brief Constructor Function.
		* @param[in] keycode Input KeyCode.
		*/
		KeyEvent(int keycode)
			:m_KeyCode(keycode)
		{}

		/**
		* @brief Input KeyCode.
		*/
		int m_KeyCode;
	};

	/**
	* @brief This Class is inherited from KeyEvent Class.
	*/
	class KeyPressedEvent : public KeyEvent
	{
	public:

		/**
		* @brief Constructor Function.
		* @param[in] keycode Input KeyCode.
		* @param[in] repeatCount Input Key repeat count.
		*/
		KeyPressedEvent(int keycode, int repeatCount)
			: KeyEvent(keycode)
			, m_RepeatCount(repeatCount)
		{}

		/**
		* @brief Destructor Function.
		*/
		virtual ~KeyPressedEvent() override = default;

		/**
		* @brief Get Input Key repeat count.
		* @return Returns the Input Key repeat count.
		*/
		inline int GetRepeatCount() const { return m_RepeatCount; }

		/**
		* @brief Serialize this Event Class to string.
		* @return Returns Serialized string.
		*/
		virtual std::string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyPressedEvent: " << m_KeyCode << " (" << m_RepeatCount << " repeats)";
			
			return ss.str();
		}

		/**
		* @brief Specific this Class Type with KeyPressed.
		*/
		EVENT_CLASS_TYPE(KeyPressed)
		
	private:

		/**
		* @brief The key repeat count.
		*/
		int m_RepeatCount;
	};

	/**
	* @brief This Class is inherited from KeyEvent Class.
	*/
	class KeyReleasedEvent : public KeyEvent
	{
	public:

		/**
		* @brief Constructor Function.
		* @param[in] keycode Input KeyCode.
		*/
		KeyReleasedEvent(int keycode)
			: KeyEvent(keycode)
		{}

		/**
		* @brief Destructor Function.
		*/
		virtual ~KeyReleasedEvent() override = default;

		/**
		* @brief Serialize this Event Class to string.
		* @return Returns Serialized string.
		*/
		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyReleasedEvent: " << m_KeyCode;
			
			return ss.str();
		}

		/**
		* @brief Specific this Class Type with KeyReleased.
		*/
		EVENT_CLASS_TYPE(KeyReleased)
	};

	/**
	* @brief This Class is inherited from KeyEvent Class.
	*/
	class KeyTypedEvent : public KeyEvent
	{
	public:

		/**
		* @brief Constructor Function.
		* @param[in] keycode Input KeyCode.
		*/
		KeyTypedEvent(int keycode)
			: KeyEvent(keycode)
		{}

		/**
		* @brief Destructor Function.
		*/
		virtual ~KeyTypedEvent() override = default;

		/**
		* @brief Serialize this Event Class to string.
		* @return Returns Serialized string.
		*/
		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyTypedEvent: " << m_KeyCode;
			
			return ss.str();
		}

		/**
		* @brief Specific this Class Type with KeyTyped.
		*/
		EVENT_CLASS_TYPE(KeyTyped)
	};
}