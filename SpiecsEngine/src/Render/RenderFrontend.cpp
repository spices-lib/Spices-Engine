#include "Pchheader.h"
#include "RenderFrontend.h"

namespace Spiecs {

	RenderFrontend::RenderFrontend()
	{
		m_RenderBackend = std::make_unique<VulkanRenderBackend>();
	}

	void RenderFrontend::BeginFrame(FrameInfo& frameInfo)
	{
		ZoneScoped;

		m_RenderBackend->BeginFrame(frameInfo);
	}

	void RenderFrontend::EndFrame(FrameInfo& frameInfo)
	{
		ZoneScoped;

		m_RenderBackend->EndFrame(frameInfo);
	}

	void RenderFrontend::DrawTest(TimeStep& ts, FrameInfo& frameInfo)
	{
		ZoneScoped;

		m_RenderBackend->DrawTest(ts, frameInfo);
	}

	void RenderFrontend::OnEvent(Event& event)
	{
		ZoneScoped;

		m_RenderBackend->OnEvent(event);
	}
}