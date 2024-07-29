/**
* @file RuntimeWorld.h.
* @brief The RuntimeWorld Class Definitions.
* @author Spices.
*/

#pragma once
#include "Core/Core.h"
#include "World.h"

namespace Spices {

	/**
	* @brief RuntimeWorld Class.
	* This class defines the specific behave of RuntimeWorld.
	*/
	class RuntimeWorld : World
	{
	public:

		/**
		* @brief Constructor Function.
		*/
		RuntimeWorld() = default;

		/**
		* @brief Destructor Function.
		*/
		virtual ~RuntimeWorld() override = default;

		/**
		* @brief This interface define the specific world behave before on activated.
		*/
		virtual void OnPreActivate() override;

		/**
		* @brief This interface define the specific world behave on activated.
		* @param[in] ts TimeStep.
		*/
		virtual void OnActivate(TimeStep& ts) override;

		/**
		* @brief This interface defines the specific world behave after on activated.
		*/
		virtual void OnDeactivate() override;

	private:

	};
}