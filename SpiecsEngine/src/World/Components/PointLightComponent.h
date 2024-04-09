#pragma once
#include "Core/Core.h"
#include "Component.h"
#include "World/PointLight.h"

namespace Spiecs {

	class PointLightComponent : Component
	{
	public:
		PointLightComponent() {};
		virtual ~PointLightComponent() {};

		virtual void OnSerialize() = 0;
		virtual void OnDeSerialize() = 0;
		virtual void OnComponentAdded() = 0;

	private:
		std::unique_ptr<PointLight> m_PointLight;
	};

}