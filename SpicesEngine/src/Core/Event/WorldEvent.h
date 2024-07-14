/**
* @file WorldEvent.h.
* @brief The WorldEvent Class Definitions.
* @author The Cherno.
*/

#pragma once
#include "Core/Core.h"
#include "Event.h"

namespace Spices {

	/**
	* @brief This Class is inherited from Event Class.
	* Inherit from it and create specific KeyEvent class. 
	* Called when WorldMarkQuery tick.
	* This Event register by ...
	*/
	class WorldEvent : public Event
	{
	public:

		/**
		* @brief Constructor Function.
		*/
		WorldEvent() = default;

		/**
		* @brief Destructor Function.
		*/
		virtual ~WorldEvent() override = default;

		/**
		* @brief Specific this Class Category with EventCategoryWorld.
		*/
		EVENT_CLASS_CATEGORY(EventCategoryWorld)
	};

	/**
	* @brief This Class is inherited from Event Class.
	*/
	class MeshAddedWorldEvent : public WorldEvent
	{
	public:

		/**
		* @brief Constructor Function.
		*/
		MeshAddedWorldEvent() = default;

		/**
		* @brief Destructor Function.
		*/
		virtual ~MeshAddedWorldEvent() override = default;

		/**
		* @brief Serialize this Event Class to string.
		* @return Returns Serialized string.
		*/
		virtual std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MeshAddedWorldEvent";
			
			return ss.str();
		}

		/**
		* @brief Specific this Class Type with MeshAdded.
		*/
		EVENT_CLASS_TYPE(MeshAdded)
	};
}