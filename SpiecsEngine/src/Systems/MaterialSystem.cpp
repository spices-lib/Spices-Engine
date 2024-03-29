#include "pchheader.h"
#include "MaterialSystem.h"

namespace Spiecs {

	void MaterialSystem::OnSystemInitialize()
	{
	}
	void MaterialSystem::OnSystemShutDown()
	{
	}
	void MaterialSystem::OnSystemUpdate()
	{
		std::this_thread::sleep_for(std::chrono::seconds(3));
		SPIECS_LOG("MaterialSystem :: fin");
	}
}