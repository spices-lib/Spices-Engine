#include "Pchheader.h"
#include "SlateFrontend.h"

namespace Spiecs {

	SlateFrontend::SlateFrontend()
	{
		m_SlateBackend = std::make_unique<ImguiSlateBackend>();
	}

	void SlateFrontend::BeginFrame(FrameInfo& frameInfo)
	{
		m_SlateBackend->BeginFrame(frameInfo);
	}

	void SlateFrontend::EndFrame(FrameInfo& frameInfo)
	{
		m_SlateBackend->EndFrame(frameInfo);
	}

	void SlateFrontend::DrawTest(FrameInfo& frameInfo)
	{
		m_SlateBackend->DrawTest(frameInfo);
	}
}