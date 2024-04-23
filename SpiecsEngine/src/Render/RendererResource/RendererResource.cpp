#include "Pchheader.h"
#include "RendererResource.h"

namespace Spiecs {

	RendererResource::RendererResource(const RendererResourceCreateInfo& info)
	{
		m_Texture = std::make_shared<Texture2D>(info);
	}

	void RendererResource::OnResized()
	{
		if (!isResizeable) return;


	}
}