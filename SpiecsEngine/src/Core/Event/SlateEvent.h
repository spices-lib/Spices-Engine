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
		SlateResizeEvent(unsigned int width, unsigned int height)
			: m_Width(width), m_Height(height) {}

		inline unsigned int GetWidth() const { return m_Width; }
		inline unsigned int GetHeight() const { return m_Height; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "SlateResizeEvent: " << m_Width << ", " << m_Height;
			return ss.str();
		}

		EVENT_CLASS_TYPE(SlateResize)
		EVENT_CLASS_CATEGORY(EventCategorySlate)

	private:
		unsigned int m_Width, m_Height;
	};
}