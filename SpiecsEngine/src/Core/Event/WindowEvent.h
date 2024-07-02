/**
* @file WindowEvent.h.
* @brief The WindowEvent Class Definitions.
* @author The Cherno.
*/

#pragma once
#include "Core/Core.h"
#include "Event.h"

namespace Spiecs {

	/**
	* @brief This Class is inherited from Event Class.
	* Called when window resized.
	* This Event registed by glfw window.
	*/
	class WindowResizeEvent : public Event
	{
	public:

		/**
		* @brief Constructor Function.
		* @param[in] width New Viewport Size.
		* @param[in] height New Viewport Size.
		*/
		WindowResizeEvent(uint32_t width, uint32_t height)
			: m_Width(width)
		    , m_Height(height)
		{}

		/**
		* @brief Destructor Function.
		*/
		virtual ~WindowResizeEvent() override = default;

		/**
		* @brief Get New Windows Width.
		* @return Returns New Windows Width.
		*/
		inline uint32_t GetWidth() const { return m_Width; }

		/**
		* @brief Get New Windows Height.
		* @return Returns New Windows Height.
		*/
		inline uint32_t GetHeight() const { return m_Height; }

		/**
		* @brief Serialize this Event Class to string.
		* @return Returns Serialized string.
		*/
		virtual std::string ToString() const override
		{
			std::stringstream ss;
			ss << "WindowResizeEvent: " << m_Width << ", " << m_Height;
			
			return ss.str();
		}

		/**
		* @brief Specific this Class Type with WindowResize.
		*/
		EVENT_CLASS_TYPE(WindowResize)

		/**
		* @brief Specific this Class Category with EventCategoryApplication.
		*/
		EVENT_CLASS_CATEGORY(EventCategoryApplication)

	private:

		/**
		* @brief New Windows Width.
		*/
		uint32_t m_Width;

		/**
		* @brief New Windows Height.
		*/
		uint32_t m_Height;
	};

	/**
	* @brief This Class is inherited from Event Class.
	*/
	class WindowCloseEvent : public Event
	{
	public:

		/**
		* @brief Constructor Function.
		*/
		WindowCloseEvent() = default;

		/**
		* @brief Destructor Function.
		*/
		virtual ~WindowCloseEvent() override = default;
		
		/**
		* @brief Specific this Class Type with WindowClose.
		*/
		EVENT_CLASS_TYPE(WindowClose)

		/**
		* @brief Specific this Class Category with EventCategoryApplication.
		*/
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};

	/**
	* @breif This Class is inherited from Event Class.
	* Called when window resized over.
	* This Event registed by VkSwapchain.
	*/
	class WindowResizeOverEvent : public Event
	{
	public:

		/**
		* @brief Constructor Function.
		* @param[in] width New Viewport Size.
		* @param[in] height New Viewport Size.
		*/
		WindowResizeOverEvent(uint32_t width, uint32_t height)
			: m_Width(width)
		    , m_Height(height)
		{}

		/**
		* @brief Destructor Function.
		*/
		virtual ~WindowResizeOverEvent() override = default;

		/**
		* @brief Get New Windows Width.
		* @return Returns New Windows Width.
		*/
		inline uint32_t GetWidth() const { return m_Width; }

		/**
		* @brief Get New Windows Height.
		* @return Returns New Windows Height.
		*/
		inline uint32_t GetHeight() const { return m_Height; }

		/**
		* @brief Serialize this Event Class to string.
		* @return Returns Serialized string.
		*/
		virtual std::string ToString() const override
		{
			std::stringstream ss;
			ss << "WindowResizeOverEvent: " << m_Width << ", " << m_Height;
			
			return ss.str();
		}

		/**
		* @brief Specific this Class Type with WindowResizeOver.
		*/
		EVENT_CLASS_TYPE(WindowResizeOver)

		/**
		* @brief Specific this Class Category with EventCategoryApplication.
		*/
		EVENT_CLASS_CATEGORY(EventCategoryApplication)

	private:

		/**
		* @brief New Windows Width.
		*/
		uint32_t m_Width;

		/**
		* @brief New Windows Height.
		*/
		uint32_t m_Height;
	};
}