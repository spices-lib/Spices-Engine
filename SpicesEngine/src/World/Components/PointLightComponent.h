/**
* @file PointLightComponent.h.
* @brief The PointLightComponent Class Definitions.
* @author Spices.
*/

#pragma once
#include "Core/Core.h"
#include "Component.h"
#include "../../../assets/Shaders/src/Header/ShaderCommon.h"

#include "glm/glm.hpp"

namespace Spices {

	/**
	* @brief PointLightComponent Class.
	* This class defines the specific behaves of PointLightComponent.
	*/
	class PointLightComponent : public Component
	{
	public:

		/**
		* @brief Constructor Function.
		* Init class variable.
		* Usually call it.
		* @param[in] pointLight Use a pointLight struct Init this component.
		*/
		PointLightComponent(const SpicesShader::PointLight& pointLight)
			: m_PointLight(pointLight)
		{}

		PointLightComponent()
			: m_PointLight{ glm::vec3(0.0f), glm::vec3(1.0f), 1.0f, 1.0f, 0.35f, 0.44f }
		{}

		/**
		* @brief Destructor Function.
		*/
		virtual ~PointLightComponent() override = default;

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
		* @brief Set PointLight Color.
		* @param[in] color PointLight Color.
		*/
		void SetColor(const glm::vec3& color) { m_PointLight.color = color; }

		/**
		* @brief Get the PointLight variable.
		* @return Returns the PointLight variable.
		*/
		const SpicesShader::PointLight& GetLight() const { return m_PointLight; }

		/**
		* @brief Set PointLight Intensity.
		* @param[in] intensity PointLight Intensity.
		*/
		void SetIntensity(float intensity) { m_PointLight.intensity = intensity; }

	private:

		/**
		* @brief This PointLight data this component handled.
		*/
		SpicesShader::PointLight m_PointLight{};
	};
}