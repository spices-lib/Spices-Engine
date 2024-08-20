/**
* @file SpriteComponent.h.
* @brief The SpriteComponent Class Definitions.
* @author Spices.
*/

#pragma once
#include "Core/Core.h"
#include "World/Components/Component.h"
#include "Resources/Mesh/Mesh.h"

namespace Spices {

	/**
	* @brief SpriteComponent Class.
	* This class defines the specific behaves of SpriteComponent.
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
		virtual ~SpriteComponent() override = default;

		/**
		* @brief This interface defines how to serialize.
		* @todo Finish it.
		*/
		virtual void OnSerialize() override {}

		/**
		* @brief This interface defines how to deserialize.
		* @todo Finish it.
		*/
		virtual void OnDeSerialize() override {};

		/**
		* @brief Set Sprite Material.
		* @param[in] material Sprite Material Path.
		*/
		void SetMaterial(const std::string& materialPath);

		/**
		* @brief Get the Mesh variable.
		* @return Returns the Mesh variable.
		*/
		std::shared_ptr<Mesh> GetMesh() { return m_Mesh; }
		
	private:

		/**
		* @brief The Mesh this component handled.
		*/
		std::shared_ptr<Mesh> m_Mesh;
	};
}