/**
* @file EditorWorld.h.
* @brief The EditorWorld Class Definitions.
* @author Spices.
*/

#pragma once
#include "Core/Core.h"
#include "World.h"

namespace Spices {

	/**
	* @brief EditorWorld Class.
	* This class defines the specific behave of EditorWorld.
	*/
	class EditorWorld : public World
	{
	public:

		/**
		* @brief Constructor Function.
		*/
		EditorWorld() = default;

		/**
		* @brief Destructor Function.
		*/
		virtual ~EditorWorld() override = default;

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