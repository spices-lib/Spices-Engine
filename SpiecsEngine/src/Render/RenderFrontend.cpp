#include "Pchheader.h"
#include "RenderFrontend.h"

namespace Spiecs {

	RenderFrontend::RenderFrontend()
	{
		m_RenderBackend = std::make_unique<VulkanRenderBackend>();
	}

	void RenderFrontend::BeginFrame(FrameInfo& frameInfo)
	{
		m_RenderBackend->BeginFrame(frameInfo);
	}

	void RenderFrontend::EndFrame(FrameInfo& frameInfo)
	{
		m_RenderBackend->EndFrame(frameInfo);
	}

	void RenderFrontend::DrawTest(TimeStep& ts, FrameInfo& frameInfo)
	{
		m_RenderBackend->DrawTest(ts, frameInfo);
	}

	void RenderFrontend::OnEvent(Event& event)
	{
		m_RenderBackend->OnEvent(event);
	}
}