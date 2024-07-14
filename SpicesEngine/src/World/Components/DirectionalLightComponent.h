/**
* @file DirectionalLightComponent.h.
* @brief The DirectionalLightComponent Class Definitions.
* @author Spices.
*/

#pragma once
#include "Core/Core.h"
#include "Component.h"
#include "../../../assets/Shaders/src/Header/ShaderCommon.h"

#include "glm/glm.hpp"

namespace Spices {

	/**
	* @brief DirectionalLightComponent Class.
	* This class defines the specific behaves of DirectionalLightComponent.
	*/
	class DirectionalLightComponent : public Component
	{
	public:

		/**
		* @brief Constructor Function.
		*/
		DirectionalLightComponent(
			const glm::vec3& color = glm::vec3(1.0f),
			float intensity = 1.0f
		)
			: m_DirectionalLight{ glm::mat4(1.0f), color, intensity }
		{}

		/**
		* @brief Constructor Function.
		* Init class variable.
		* Usually call it.
		* @param[in] directionalLight Use a DirectionalLight struct Init this component.
		*/
		DirectionalLightComponent(const SpicesShader::DirectionalLight& directionalLight)
			: m_DirectionalLight(directionalLight)
		{}

		/**
		* @brief Destructor Function.
		*/
		virtual ~DirectionalLightComponent() override = default;

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
		* @brief Get the DirectionalLight variable.
		* @return Returns the DirectionalLight variable.
		*/
		const SpicesShader::DirectionalLight& GetLight() const { return m_DirectionalLight; }

		void SetColor(const glm::vec3& color) { m_DirectionalLight.color = color; }
		void SetIntensity(float intensity) { m_DirectionalLight.intensity = intensity; }

	private:

		/**
		* @brief This DirectionalLight data this component handled.
		*/
		SpicesShader::DirectionalLight m_DirectionalLight{};
	};
}