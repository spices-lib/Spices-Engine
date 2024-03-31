#pragma once
#include "Core/Core.h"
#include "SystemManager.h"

namespace Spiecs {

	class UISystem : public System
	{
	public:
		UISystem(const std::string& systemName) : System(systemName) {};
		virtual ~UISystem() override {};

		virtual void OnSystemInitialize() override;
		virtual void OnSystemShutDown() override;
		virtual void OnSystemUpdate() override;

	private:
	};
}