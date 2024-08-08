#include "Pchheader.h"
#include "RenderFrontend.h"

namespace Spices {

	RenderFrontend::RenderFrontend()
	{
		m_RenderBackend = std::make_unique<VulkanRenderBackend>();
	}

	void RenderFrontend::BeginFrame(FrameInfo& frameInfo)
	{
		SPICES_PROFILE_ZONE;

		m_RenderBackend->BeginFrame(frameInfo);
	}

	void RenderFrontend::EndFrame(FrameInfo& frameInfo)
	{
		SPICES_PROFILE_ZONE;

		m_RenderBackend->EndFrame(frameInfo);
	}

	void RenderFrontend::RenderFrame(TimeStep& ts, FrameInfo& frameInfo)
	{
		SPICES_PROFILE_ZONE;

		m_RenderBackend->RenderFrame(ts, frameInfo);
	}

	void RenderFrontend::OnEvent(Event& event)
	{
		SPICES_PROFILE_ZONE;

		m_RenderBackend->OnEvent(event);
	}
}