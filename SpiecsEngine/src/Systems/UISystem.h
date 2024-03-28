#pragma once
#include "Core/Core.h"
#include "SystemManager.h"

namespace Spiecs {

	class UISystem : System
	{
	public:
		UISystem(const std::string& systemName) : System(systemName) {};
		virtual ~UISystem() override {};

		UISystem(const UISystem&) = delete;
		UISystem& operator=(const UISystem&) = delete;

		virtual void OnSystemInitialize() override;
		virtual void OnSystemShutDown() override;
		virtual void OnSystemUpdate() override;

	private:
	};
}