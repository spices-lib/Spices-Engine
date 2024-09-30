/**
* @file RenderSystem.cpp.
* @brief The RenderSystem Class Implementation.
* @author Spices.
*/

#include "Pchheader.h"
#include "RenderSystem.h"
#include "Resources/Mesh/Mesh.h"

namespace Spices {

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
		SPICES_PROFILE_ZONE;

		/**
		* @brief Begin Render this frame.
		*/
		m_RenderFrontend->BeginFrame(FrameInfo::Get());
		
		/**
		* @brief Render this frame.
		*/
		m_RenderFrontend->RenderFrame(ts, FrameInfo::Get());

		/**
		* @brief End Render this frame.
		*/
		m_RenderFrontend->EndFrame(FrameInfo::Get());

		/**
		* @brief Update frameindex.
		*/
		FrameInfo::Get().m_FrameIndex = (FrameInfo::Get().m_FrameIndex + 1) % MaxFrameInFlight;
		FrameInfo::Get().m_IsSkipPresent = false;

		static uint32_t nSkipFrames = FrameInfo::Get().m_NSkipPresentFrame;
		if (nSkipFrames == FrameInfo::Get().m_NSkipPresentFrame)
		{
			nSkipFrames = 0;
			FrameInfo::Get().m_NSkipPresentFrame = 0;
		}
	}

	void RenderSystem::OnEvent(Event& event)
	{
		SPICES_PROFILE_ZONE;

		m_RenderFrontend->OnEvent(event);
	}
}