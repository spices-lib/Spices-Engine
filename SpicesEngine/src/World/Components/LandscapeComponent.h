/**
* @file LandscapeComponent.h.
* @brief The LandscapeComponent Class Definitions.
* @author Spices.
*/

#pragma once
#include "Core/Core.h"
#include "Component.h"
#include "Resources/Mesh/Mesh.h"

#include <memory>

namespace Spices {

	/**
	* @brief LandscapeComponent Class.
	* This class defines the specific behaves of LandscapeComponent.
	*/
	class LandscapeComponent : public Component
	{
	public:

		/**
		* @brief Constructor Function.
		*/
		LandscapeComponent() = default;

		/**
		* @brief Destructor Function.
		*/
		virtual ~LandscapeComponent() override = default;

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

	};
}