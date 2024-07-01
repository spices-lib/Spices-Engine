#pragma once
#include "Core/Core.h"
#include "Event.h"

namespace Spiecs {

	/**
	* @brief Called when WorldMarkQueryer tick.
	* This Event registed by ...
	*/
	class WorldEvent : public Event
	{
	public:
		WorldEvent() {};

		EVENT_CLASS_CATEGORY(EventCategoryWorld)
	};

	class MeshAddedWorldEvent : public WorldEvent
	{
	public:
		MeshAddedWorldEvent() {};

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MeshAddedWorldEvent";
			return ss.str();
		}

		EVENT_CLASS_TYPE(MeshAdded)
		EVENT_CLASS_CATEGORY(EventCategoryWorld)
	};
}