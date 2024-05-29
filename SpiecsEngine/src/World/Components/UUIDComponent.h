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
	* This class defines the specific behaver of UUIDComponent.
	*/
	class UUIDComponent : public Component
	{
	public:

		/**
		* @brief Constructor Function.
		*/
		UUIDComponent() {};

		/**
		* @brief Destructor Function.
		*/
		virtual ~UUIDComponent() {};

		/**
		* @brief This interface defines how to serialize.
		* @todo Finish it.
		*/
		virtual void OnSerialize() override;

		/**
		* @brief This interface defines how to dserialize.
		* @todo Finsih it.
		*/
		virtual void OnDeSerialize() override;

		/**
		* @brief Defines Component icon on stage.
		* Set m_SlateStageIconPath here.
		* @return Returns Specific m_SlateStageIconPath.
		*/
		virtual const std::string& DefineStageIcon() override;

		/**
		* @brief Set the uuid this component handled.
		* @param[in] uuid UUID.
		*/
		void SetUUID(UUID uuid) { m_uuid = uuid; };

		/**
		* @brief Get the uuid variable.
		* @return Returns the uuid variable.
		*/
		inline const UUID GetUUID() { return m_uuid; };

	private:

		/**
		* @brief The uuid this component handled.
		*/
		UUID m_uuid;
	};
}