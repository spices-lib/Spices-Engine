#pragma once
#include "Core/Core.h"
#include "SystemManager.h"

namespace Spiecs {

	class NativeScriptSystem : public System
	{
	public:
		NativeScriptSystem(const std::string& systemName) : System(systemName) {};
		virtual ~NativeScriptSystem() override {};

		virtual void OnSystemInitialize() override;
		virtual void OnSystemShutDown() override;
		virtual void OnSystemUpdate(TimeStep ts) override;

	private:
	};
}