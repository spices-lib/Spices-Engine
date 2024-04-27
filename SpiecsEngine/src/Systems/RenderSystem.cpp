/**
* @file RenderSystem.cpp.
* @brief The RenderSystem Class Implementation.
* @author Spiecs.
*/

#include "Pchheader.h"
#include "RenderSystem.h"
#include "Resources/Mesh/Mesh.h"

namespace Spiecs {

	void RenderSystem::OnSystemInitialize()
	{
		/**
		* @brief Init a RenderFrontend.
		*/
		m_RenderFrontend = std::make_unique<RenderFrontend>();
	}

	void RenderSystem::OnSystemShutDown()
	{

	}

	void RenderSystem::OnSystemUpdate(TimeStep& ts)
	{
		//std::cout << "FPS: " << 1000 / frameTime << std::endl;

		m_RenderFrontend->BeginFrame(FrameInfo::Get());

		m_RenderFrontend->DrawTest(FrameInfo::Get());

		m_RenderFrontend->EndFrame(FrameInfo::Get());

		FrameInfo::Get().m_FrameIndex = (FrameInfo::Get().m_FrameIndex + 1) % MaxFrameInFlight;
	}

	void RenderSystem::OnEvent(Event& event)
	{
		m_RenderFrontend->OnEvent(event);
	}
}