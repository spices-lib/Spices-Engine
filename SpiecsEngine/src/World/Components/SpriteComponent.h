/**
* @file SpriteComponent.h.
* @brief The SpriteComponent Class Definitions.
* @author Spiecs.
*/

#pragma once
#include "Core/Core.h"
#include "World/Components/Component.h"
#include "Resources/Mesh/Mesh.h"

namespace Spiecs {

	/**
	* @brief SpriteComponent Class.
	* This class defines the specific behaver of SpriteComponent.
	*/
	class SpriteComponent : public Component
	{
	public:

		/**
		* @brief Constructor Function.
		*/
		SpriteComponent();

		/**
		* @brief Destructor Function.
		*/
		virtual ~SpriteComponent() {};

		/**
		* @brief This interface defines how to serialize.
		* @todo Finish it.
		*/
		virtual void OnSerialize() override {};

		/**
		* @brief This interface defines how to dserialize.
		* @todo Finsih it.
		*/
		virtual void OnDeSerialize() override {};

	private:

		/**
		* @brief The Mesh this component handled.
		*/
		std::shared_ptr<Mesh> m_Mesh;
	};
}