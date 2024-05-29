/**
* @file DirectionalLightComponent.h.
* @brief The DirectionalLightComponent Class Definitions.
* @author Spiecs.
*/

#pragma once
#include "Core/Core.h"
#include "Component.h"
#include "glm/glm.hpp"

namespace Spiecs {

	/**
	* @brief DirectionalLightComponent Class.
	* This class defines the specific behaver of DirectionalLightComponent.
	*/
	class DirectionalLightComponent : public Component
	{
	public:

		/**
		* @brief This struct defines DirectionalLight data.
		* This struct's data needed to be transfered to shader.
		*/
		struct DirectionalLight
		{
			/**
			* @brief Direction of SunLight.
			*/
			alignas(16) glm::vec3 direction{ 1.0f };

			/**
			* @brief Color of SunLight.
			*/
			alignas(16) glm::vec3 color{ 1.0f };

			/**
			* @brief Intensity of SunLight.
			*/
			float intensity = 1.0f;
		};

	public:

		/**
		* @brief Constructor Function.
		*/
		DirectionalLightComponent() {};

		/**
		* @brief Constructor Function.
		* Init class variable.
		* Usually call it.
		* @param[in] directionalLight Use a DirectionalLight struct Init this component.
		*/
		DirectionalLightComponent(const DirectionalLight& directionalLight) : m_DirectionalLight(directionalLight) {};

		/**
		* @brief Destructor Function.
		*/
		virtual ~DirectionalLightComponent() {};

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
		*/
		virtual void DefineStageIcon() override;

		/**
		* @brief Get the DirectionalLight variable.
		* @return Returns the DirectionalLight variable.
		*/
		inline const DirectionalLight& GetLight() { return m_DirectionalLight; };

	private:

		/**
		* @brief This DirectionalLight data this component handled.
		*/
		DirectionalLight m_DirectionalLight{};
	};
}