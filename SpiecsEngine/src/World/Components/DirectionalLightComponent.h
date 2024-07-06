/**
* @file DirectionalLightComponent.h.
* @brief The DirectionalLightComponent Class Definitions.
* @author Spiecs.
*/

#pragma once
#include "Core/Core.h"
#include "Component.h"
#include "../../../assets/Shaders/src/Header/ShaderCommon.h"

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
		* @brief Constructor Function.
		*/
		DirectionalLightComponent(
			const glm::vec3& rotation = glm::vec3(0.0f),
			const glm::vec3& color = glm::vec3(1.0f),
			float intensity = 1.0f
		)
			: m_DirectionalLight{ rotation, color, intensity }
		{};

		/**
		* @brief Constructor Function.
		* Init class variable.
		* Usually call it.
		* @param[in] directionalLight Use a DirectionalLight struct Init this component.
		*/
		DirectionalLightComponent(const SpiecsShader::DirectionalLight& directionalLight)
			: m_DirectionalLight(directionalLight)
		{};

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
		* @brief Get the DirectionalLight variable.
		* @return Returns the DirectionalLight variable.
		*/
		inline const SpiecsShader::DirectionalLight& GetLight() { return m_DirectionalLight; };

		void SetColor(const glm::vec3& color) { m_DirectionalLight.color = color; };
		void SetIntensity(float intensity) { m_DirectionalLight.intensity = intensity; };

	private:

		/**
		* @brief This DirectionalLight data this component handled.
		*/
		SpiecsShader::DirectionalLight m_DirectionalLight{};
	};
}