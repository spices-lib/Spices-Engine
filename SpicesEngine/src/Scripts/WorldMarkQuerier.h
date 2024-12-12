/**
* @file WorldMarkQuerier.h.
* @brief The WorldMarkQuerier Definitions.
* @author Spices.
*/

#pragma once
#include "Core/Core.h"
#include "NativeScript.h"

namespace Spices {

	/**
	* @brief Script of handle world mark.
	*/
	class WorldMarkQuerier : public NativeScript
	{
	public:

		/**
		* @brief Constructor Function.
		*/
		WorldMarkQuerier() = default;

		/**
		* @brief Destructor Function.
		*/
		virtual ~WorldMarkQuerier() override = default;

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