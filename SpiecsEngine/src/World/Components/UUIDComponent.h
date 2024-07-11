/**
* @file UUIDComponent.h.
* @brief The UUIDComponent Class Definitions.
* @author Spiecs.
*/

#pragma once
#include "Core/Core.h"
#include "Component.h"
#include "Core/UUID.h"

namespace Spiecs {

	/**
	* @brief UUIDComponent Class.
	* This class defines the specific behaves of UUIDComponent.
	*/
	class UUIDComponent : public Component
	{
	public:

		/**
		* @brief Constructor Function.
		*/
		UUIDComponent() = default;

		/**
		* @brief Destructor Function.
		*/
		virtual ~UUIDComponent() override = default;

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
		* @brief Set the uuid this component handled.
		* @param[in] uuid UUID.
		*/
		void SetUUID(UUID uuid) { m_uuid = uuid; }

		/**
		* @brief Get the uuid variable.
		* @return Returns the uuid variable.
		*/
		const UUID& GetUUID() const { return m_uuid; }

	private:

		/**
		* @brief The uuid this component handled.
		*/
		UUID m_uuid;
	};
}