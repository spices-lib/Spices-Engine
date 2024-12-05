/**
* @file EntityComponent.h.
* @brief The EntityComponent Class Definitions.
* @author Spices.
*/

#pragma once
#include "Core/Core.h"
#include "Component.h"

namespace Spices {

	/**
	* @brief EntityComponent Class.
	* This class defines the specific behaves of EntityComponent.
	*/
	class EntityComponent : public Component
	{
	public:

		/**
		* @brief Constructor Function.
		*/
		EntityComponent()
		{}

		/**
		* @brief Destructor Function.
		*/
		virtual ~EntityComponent() override = default;

		/**
		* @brief This interface defines how to serialize.
		* @todo Finish it.
		*/
		virtual void OnSerialize() override;

		/**
		* @brief This interface defines how to deserialize.
		* @todo Finish it.
		*/
		virtual void OnDeSerialize() override;

		/**
		* @brief This interface defines how to draw this component to property panel.
		*/
		virtual void DrawThis() override;
		


	private:


	};
}