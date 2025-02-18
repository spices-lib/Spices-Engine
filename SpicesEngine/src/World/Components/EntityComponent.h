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
		EntityComponent() = default;

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
		
		/**
		* @brief Add an child entity to this Component.
		* @param[in] entity child entity.
		*/
		void AddEntity(uint32_t entity);

		/**
		* @brief Remove an child entity from this Component.
		* @param[in] entity child entity.
		*/
		void RemoveEntity(uint32_t entity);

		/**
		* @brief Get this Component Entities.
		* @return Returns this Component Entities.
		*/
		const std::set<uint32_t>& GetEntities() { return m_Entities; }

	private:

		/**
		* @brief child entities.
		*/
		std::set<uint32_t> m_Entities;

	};
}