#include "pchheader.h"
#include "RenderFrontend.h"

namespace Spiecs {

	RenderFrontend::RenderFrontend()
	{
		m_RenderBackend = std::make_unique<VulkanRenderBackend>();
	}
}