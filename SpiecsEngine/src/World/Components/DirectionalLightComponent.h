#pragma once
#include "Core/Core.h"
#include "Component.h"
#include "glm/glm.hpp"

namespace Spiecs {

	class DirectionalLightComponent : public Component
	{
	public:
		struct DirectionalLight
		{
			alignas(16) glm::vec3 direction{ 1.0f };
			alignas(16) glm::vec3 color{ 1.0f };
			float intensity = 1.0f;
		};

	public:
		DirectionalLightComponent() {};
		DirectionalLightComponent(DirectionalLight directionalLight) : m_DirectionalLight(directionalLight) {};

		virtual ~DirectionalLightComponent() {};

		virtual void OnSerialize() override;
		virtual void OnDeSerialize() override;

		DirectionalLight& GetLight() { return m_DirectionalLight; };

	private:
		DirectionalLight m_DirectionalLight{};
	};
}