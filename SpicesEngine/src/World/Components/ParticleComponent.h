/**
* @file ParticleComponent.h.
* @brief The ParticleComponent Class Definitions.
* @author Spices.
*/

#pragma once
#include "Core/Core.h"
#include "Component.h"

namespace Spices {

	/**
	* @brief ParticleComponent Class.
	* This class defines the specific behaves of ParticleComponent.
	*/
	class ParticleComponent : public Component
	{
	public:

		/**
		* @brief Constructor Function.
		*/
		ParticleComponent() = default;

		/**
		* @brief Constructor Function.
		*/
		ParticleComponent(const std::string& tag);

		/**
		* @brief Destructor Function.
		*/
		virtual ~ParticleComponent() override = default;

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