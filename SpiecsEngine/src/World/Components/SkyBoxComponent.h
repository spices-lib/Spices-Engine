#pragma once
#include "Core/Core.h"
#include "Component.h"

namespace Spiecs {

	class SkyBoxComponent : public Component
	{
	public:
		SkyBoxComponent() {};
		SkyBoxComponent(const std::string& path) : m_MaterialPath(path) {};
		virtual ~SkyBoxComponent() {};

		virtual void OnSerialize() override;
		virtual void OnDeSerialize() override;
		virtual void OnComponentAdded() override;

	private:
		std::string m_MaterialPath;
	};
}