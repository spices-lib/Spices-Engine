#pragma once
#include "Core/Core.h"
#include "SystemManager.h"

namespace Spiecs {

	class MeshSystem : public System
	{
	public:
		MeshSystem(const std::string& systemName) : System(systemName) {};
		virtual ~MeshSystem() override {};

		MeshSystem(const MeshSystem&) = delete;
		MeshSystem& operator=(const MeshSystem&) = delete;

		virtual void OnSystemInitialize() override;
		virtual void OnSystemShutDown() override;
		virtual void OnSystemUpdate() override;

	private:
	};
}