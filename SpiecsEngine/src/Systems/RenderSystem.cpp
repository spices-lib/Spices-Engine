#include "Pchheader.h"
#include "RenderSystem.h"
#include "Render/Resources/Mesh.h"

namespace Spiecs {

	void RenderSystem::OnSystemInitialize()
	{
		m_RenderFrontend = std::make_unique<RenderFrontend>();


		SPIECS_LOG("RenderSystem: Init Succeed");
	}

	void RenderSystem::OnSystemShutDown()
	{

	}

	void RenderSystem::OnSystemUpdate(TimeStep& ts)
	{
		

		//std::cout << "FPS: " << 1000 / frameTime << std::endl;

		m_RenderFrontend->beginFrame(FrameInfo::Get());

		m_RenderFrontend->DrawTest(FrameInfo::Get());

		m_RenderFrontend->endFrame(FrameInfo::Get());

		FrameInfo::Get().m_FrameIndex = (FrameInfo::Get().m_FrameIndex + 1) % MaxFrameInFlight;
	}
}