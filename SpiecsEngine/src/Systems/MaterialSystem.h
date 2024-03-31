#pragma once
#include "Core/Core.h"
#include "SystemManager.h"

namespace Spiecs {

	class MaterialSystem : public System
	{
	public:
		MaterialSystem(const std::string& systemName) : System(systemName) {};
		virtual ~MaterialSystem() override {};

		virtual void OnSystemInitialize() override;
		virtual void OnSystemShutDown() override;
		virtual void OnSystemUpdate() override;

	private:
	};
}