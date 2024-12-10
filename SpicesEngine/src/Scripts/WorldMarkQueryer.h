/**
* @file WorldMarkQueryer.h.
* @brief The WorldMarkQueryer Definitions.
* @author Spices.
*/

#pragma once
#include "Core/Core.h"
#include "NativeScript.h"

namespace Spices {

	/**
	* @brief Script of handle world mark.
	*/
	class WorldMarkQueryer : public NativeScript
	{
	public:

		/**
		* @brief Constructor Function.
		*/
		WorldMarkQueryer() = default;

		/**
		* @brief Destructor Function.
		*/
		virtual ~WorldMarkQueryer() override = default;

		/**
		* @brief This interface defines the behave on specific component tick every frame.
		* @param[in] ts TimeStep.
		*/
		virtual void OnTick(TimeStep& ts) override;

		/**
		* @brief This interface defines the behave on specific component event happened.
		* @param[in] e Event.
		*/
		virtual void OnEvent(Event& e) override {};
	};
}