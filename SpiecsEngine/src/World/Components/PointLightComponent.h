#pragma once
#include "Core/Core.h"
#include "Component.h"
#include "glm/glm.hpp"

namespace Spiecs {

	class PointLightComponent : public Component
	{
	public:
		struct PointLight
		{
			alignas(16) glm::vec3 color{ 1.0f };
			alignas(16) glm::vec3 position{ 0.0f };
			float intensity = 1.0;
			float constantf = 1.0;
			float linear = 0.35;
			float quadratic = 0.44;
		};

	public:
		PointLightComponent() {};
		PointLightComponent(const PointLight& pointLight) : m_PointLight(pointLight) {};

		virtual ~PointLightComponent() {};

		virtual void OnSerialize() override;
		virtual void OnDeSerialize() override;

		PointLight& GetLight() { return m_PointLight; };

	private:
		PointLight m_PointLight{};
	};
}