#pragma once
#include "Event.h"

namespace Spiecs {

	/**
	* @brief Called when window resized over.
	* This Event registed by ImguiViewport.
	*/
	class SlateResizeEvent : public Event
	{
	public:
		SlateResizeEvent(uint32_t width, uint32_t height)
			: m_Width(width), m_Height(height) {}

		inline uint32_t GetWidth() const { return m_Width; }
		inline uint32_t GetHeight() const { return m_Height; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "SlateResizeEvent: " << m_Width << ", " << m_Height;
			return ss.str();
		}

		EVENT_CLASS_TYPE(SlateResize)
		EVENT_CLASS_CATEGORY(EventCategorySlate)

	private:
		uint32_t m_Width, m_Height;
	};
}