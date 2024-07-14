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
	* This class defines the specific behaver of RuntimeWorld.
	*/
	class RuntimeWorld : World
	{
	public:

		/**
		* @brief Constructor Function.
		*/
		RuntimeWorld() {};

		/**
		* @brief Destructor Function.
		*/
		virtual ~RuntimeWorld() {};

		/**
		* @brief This interface define the specific world behaver before on actived.
		*/
		virtual void OnPreActivate() override;

		/**
		* @brief This interface define the specific world behaver on actived.
		* @param[in] ts TimeStep.
		*/
		virtual void OnActivate(TimeStep& ts) override;

		/**
		* @brief This interface defines the specific world behaver after on actived.
		*/
		virtual void OnDeactivate() override;

	private:

	};
}