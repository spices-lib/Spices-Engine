#include "pchheader.h"
#include "RenderSystem.h"

namespace Spiecs {

	void RenderSystem::OnSystemInitialize()
	{
		m_RenderFrontend = std::make_unique<RenderFrontend>();

		SPIECS_LOG("RenderSystem: Init Succeed");
	}

	void RenderSystem::OnSystemShutDown()
	{

	}

	void RenderSystem::OnSystemUpdate()
	{

	}

}