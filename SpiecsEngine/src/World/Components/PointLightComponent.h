/**
* @file PointLightComponent.h.
* @brief The PointLightComponent Class Definitions.
* @author Spiecs.
*/

#pragma once
#include "Core/Core.h"
#include "Component.h"
#include "../../../assets/Shaders/src/Header/ShaderCommon.h"

#include "glm/glm.hpp"

namespace Spiecs {

	/**
	* @brief PointLightComponent Class.
	* This class defines the specific behaver of PointLightComponent.
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
		PointLightComponent(const SpiecsShader::PointLight& pointLight) : m_PointLight(pointLight) {};

		PointLightComponent()
			: m_PointLight{ glm::vec3(0.0f), glm::vec3(1.0f), 1.0f, 1.0f, 0.35f, 0.44f }
		{};

		/**
		* @brief Destructor Function.
		*/
		virtual ~PointLightComponent() {};

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
		* @brief Set PointLight Color.
		* @param[in] color PointLight Color.
		*/
		void SetColor(glm::vec3 color) { m_PointLight.color = color; };

		/**
		* @brief Get the PointLight variable.
		* @return Returns the PointLight variable.
		*/
		inline const SpiecsShader::PointLight& GetLight() { return m_PointLight; };

	private:

		/**
		* @brief This PointLight data this component handled.
		*/
		SpiecsShader::PointLight m_PointLight{};
	};
}