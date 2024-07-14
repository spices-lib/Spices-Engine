/**
* @file MouseEvent.h.
* @brief The MouseEvent Class Definitions.
* @author The Cherno.
*/

#pragma once
#include "Core/Core.h"
#include "Event.h"

namespace Spices {

	/**
	* @brief This Class is inherited from Event Class.
	*/
	class MouseMovedEvent : public Event
	{
	public:

		/**
		* @brief Constructor Function.
		* @param[in] x Mouse Position X.
		* @param[in] y Mouse Position Y.
		*/
		MouseMovedEvent(float x, float y)
			: m_MouseX(x)
		    , m_MouseY(y)
		{}

		/**
		* @brief Destructor Function.
		*/
		virtual ~MouseMovedEvent() override = default;

		/**
		* @brief Get Mouse Position X.
		* @return Returns Mouse Position X.
		*/
		const float& GetX() const { return m_MouseX; }

		/**
		* @brief Get Mouse Position Y.
		* @return Returns Mouse Position Y.
		*/
		const float& GetY() const { return m_MouseY; }

		/**
		* @brief Serialize this Event Class to string.
		* @return Returns Serialized string.
		*/
		virtual std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseMovedEvent: " << m_MouseX << " ," << m_MouseY;
			
			return ss.str();
		}

		/**
		* @brief Specific this Class Type with MouseMoved.
		*/
		EVENT_CLASS_TYPE(MouseMoved)

		/**
		* @brief Specific this Class Category with EventCategoryMouse and EventCategoryInput.
		*/
		EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)

	private:

		/**
		* @brief Mouse Position X.
		*/
		float m_MouseX;

		/**
		* @brief Mouse Position Y.
		*/
		float m_MouseY;
	};

	/**
	* @brief This Class is inherited from Event Class.
	*/
	class MouseScrolledEvent : public Event
	{
	public:

		/**
		* @brief Constructor Function.
		* @param[in] xOffset Mouse offset in X.
		* @param[in] yOffset Mouse offset in Y.
		*/
		MouseScrolledEvent(float xOffset, float yOffset)
			: m_XOffset(xOffset)
		    , m_YOffset(yOffset)
		{}

		/**
		* @brief Destructor Function.
		*/
		virtual ~MouseScrolledEvent() override = default;

		/**
		* @brief Get Mouse offset in X.
		* @return Returns Mouse offset in X.
		*/
		const float& GetXOffset() const { return m_XOffset; }

		/**
		* @brief Get Mouse offset in Y.
		* @return Returns Mouse offset in Y.
		*/
		const float& GetYOffset() const { return m_YOffset; }

		/**
		* @brief Serialize this Event Class to string.
		* @return Returns Serialized string.
		*/
		virtual std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseScrolledEvent: " << GetXOffset() << ", " << GetYOffset();
			
			return ss.str();
		}

		/**
		* @brief Specific this Class Type with MouseScrolled.
		*/
		EVENT_CLASS_TYPE(MouseScrolled)

		/**
		* @brief Specific this Class Category with EventCategoryMouse and EventCategoryInput.
		*/
		EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)
		
	private:

		/**
		* @brief Mouse offset in X.
		*/
		float m_XOffset;

		/**
		* @brief Mouse offset in Y.
		*/
		float m_YOffset;
	};

	/**
	* @brief This Class is inherited from Event Class.
	* Inherit from it and create specific MouseButtonEvent class. 
	*/
	class MouseButtonEvent : public Event
	{
	public:

		/**
		* @brief Destructor Function.
		*/
		virtual ~MouseButtonEvent() override = default;

		/**
		* @brief Destructor Function.
		*/
		const int& GetMouseButton() const { return m_Button; }

		/**
		* @brief Specific this Class Category with EventCategoryMouse and EventCategoryInput.
		*/
		EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)
		
	protected:

		/**
		* @brief Constructor Function.
		* @param[in] button Input Mouse Button.
		*/
		MouseButtonEvent(int button)
			: m_Button(button)
		{}

		/**
		* @brief Input Mouse Button.
		*/
		int m_Button;
	};

	/**
	* @brief This Class is inherited from Event Class.
	*/
	class MouseButtonPressedEvent : public MouseButtonEvent
	{
	public:

		/**
		* @brief Constructor Function.
		* @param[in] button Input Mouse Button.
		*/
		MouseButtonPressedEvent(int button)
			: MouseButtonEvent(button)
		{}

		/**
		* @brief Destructor Function.
		*/
		virtual ~MouseButtonPressedEvent() override = default;

		/**
		* @brief Serialize this Event Class to string.
		* @return Returns Serialized string.
		*/
		virtual std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseButtonPressedEvent: " << m_Button;
			
			return ss.str();
		}

		/**
		* @brief Specific this Class Type MouseButtonPressed.
		*/
		EVENT_CLASS_TYPE(MouseButtonPressed)
	};

	/**
	* @brief This Class is inherited from Event Class.
	*/
	class MouseButtonReleasedEvent : public MouseButtonEvent
	{
	public:

		/**
		* @brief Constructor Function.
		* @param[in] button Input Mouse Button.
		*/
		MouseButtonReleasedEvent(int button)
			: MouseButtonEvent(button)
		{}

		/**
		* @brief Destructor Function.
		*/
		virtual ~MouseButtonReleasedEvent() override = default;

		/**
		* @brief Serialize this Event Class to string.
		* @return Returns Serialized string.
		*/
		virtual std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseButtonReleasedEvent: " << m_Button;
			
			return ss.str();
		}

		/**
		* @brief Specific this Class Type MouseButtonReleased.
		*/
		EVENT_CLASS_TYPE(MouseButtonReleased)
	};
}