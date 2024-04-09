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
			glm::vec3 m_Direction{ 1.0f };
			glm::vec3 m_Color{ 1.0f };
			float m_Intensity = 1.0f;
		};

	public:
		DirectionalLightComponent() {};
		DirectionalLightComponent(DirectionalLight directionalLight) : m_DirectionalLight(directionalLight) {};

		virtual ~DirectionalLightComponent() {};

		virtual void OnSerialize() override;
		virtual void OnDeSerialize() override;
		virtual void OnComponentAdded() override;

		DirectionalLight& GetLight() { return m_DirectionalLight; };

	private:
		DirectionalLight m_DirectionalLight{};
	};
}