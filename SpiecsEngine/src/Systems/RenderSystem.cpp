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
		/**
		* @brief Begin Render this frame.
		*/
		m_RenderFrontend->BeginFrame(FrameInfo::Get());
		
		/**
		* @brief Render this frame.
		*/
		m_RenderFrontend->DrawTest(ts, FrameInfo::Get());

		/**
		* @brief End Render this frame.
		*/
		m_RenderFrontend->EndFrame(FrameInfo::Get());

		/**
		* @brief Update frameindex.
		*/
		FrameInfo::Get().m_FrameIndex = (FrameInfo::Get().m_FrameIndex + 1) % MaxFrameInFlight;
	}

	void RenderSystem::OnEvent(Event& event)
	{
		m_RenderFrontend->OnEvent(event);
	}
}