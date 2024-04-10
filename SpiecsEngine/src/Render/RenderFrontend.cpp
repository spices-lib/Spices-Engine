#include "Pchheader.h"
#include "RenderFrontend.h"

namespace Spiecs {

	RenderFrontend::RenderFrontend()
	{
		m_RenderBackend = std::make_unique<VulkanRenderBackend>();
	}

	void RenderFrontend::beginFrame(FrameInfo& frameInfo)
	{
		m_RenderBackend->beginFrame(frameInfo);
	}

	void RenderFrontend::endFrame(FrameInfo& frameInfo)
	{
		m_RenderBackend->endFrame(frameInfo);
	}

	void RenderFrontend::DrawTest(FrameInfo& frameInfo)
	{
		m_RenderBackend->DrawTest(frameInfo);
	}
}