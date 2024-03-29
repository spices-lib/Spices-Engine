#include "pchheader.h"
#include "MeshSystem.h"

namespace Spiecs {

	void MeshSystem::OnSystemInitialize()
	{
	}
	void MeshSystem::OnSystemShutDown()
	{
	}
	void MeshSystem::OnSystemUpdate()
	{
		std::this_thread::sleep_for(std::chrono::seconds(2));
		SPIECS_LOG("MeshSystem :: fin");
	}
}