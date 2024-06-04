/**
* @file PointLightComponent.h.
* @brief The PointLightComponent Class Definitions.
* @author Spiecs.
*/

#pragma once
#include "Core/Core.h"
#include "Component.h"
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
		* @brief This struct defines PointLight data.
		* This struct's data needed to be transfered to shader.
		*/
		struct PointLight
		{
			/**
			* @brief World Position of PointLight.
			*/
			alignas(16) glm::vec3 position{ 0.0f };

			/**
			* @brief Color of PointLight.
			*/
			alignas(16) glm::vec3 color{ 1.0f };

			/**
			* @brief Intensity of PointLight.
			*/
			float intensity = 1.0f;

			/**
			* @brief Constantf of PointLight.
			*/
			float constantf = 1.0f;

			/**
			* @brief Linear of PointLight.
			*/
			float linear = 0.35f;

			/**
			* @brief Quadratic of PointLight.
			*/
			float quadratic = 0.44f;
		};

	public:

		/**
		* @brief Constructor Function.
		* Init class variable.
		* Usually call it.
		* @param[in] pointLight Use a pointLight struct Init this component.
		*/
		PointLightComponent(const PointLight& pointLight) : m_PointLight(pointLight) {};

		PointLightComponent(
			const glm::vec3& color = glm::vec3(1.0f),
			float intensity = 1.0f,
			float constantf = 1.0f,
			float linear = 0.35f,
			float quadratic = 0.44f
		)
			: m_PointLight{glm::vec3(0.0f), color, intensity, constantf, linear, quadratic}
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
		* @brief Get the PointLight variable.
		* @return Returns the PointLight variable.
		*/
		inline const PointLight& GetLight() { return m_PointLight; };

	private:

		/**
		* @brief This PointLight data this component handled.
		*/
		PointLight m_PointLight{};
	};
}