#pragma once
#include "Core/Core.h"
#include "SystemManager.h"

namespace Spiecs {

	/**
	* @brief handle resource load/unload event.
	* resource garbage collection
	*/
	class ResourceSystem : public System
	{
	public:
		ResourceSystem(const std::string& systemName) : System(systemName) {};
		virtual ~ResourceSystem() override {};

		virtual void OnSystemInitialize() override;
		virtual void OnSystemShutDown() override;
		virtual void OnSystemUpdate(TimeStep& ts) override;

	private:
	};
}