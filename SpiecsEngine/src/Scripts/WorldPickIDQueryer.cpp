#include "Pchheader.h"
#include "WorldPickIDQueryer.h"
#include "Systems/SlateSystem.h"
#include "Slate/Imgui/ViewPort/ImguiViewport.h"
#include "Render/Vulkan/VulkanRenderBackend.h"

namespace Spiecs {

	WorldPickIDQueryer::WorldPickIDQueryer()
	{
		m_WorldPickIDMemory = &m_WorldPickID[0];
	}

	void WorldPickIDQueryer::OnTick(TimeStep& ts)
	{
		/**
		* @brief The first frame, we will not get register pointer.
		*/
		if (!SlateSystem::GetRegister()) return;

		if (!m_ViewPort) m_ViewPort = SlateSystem::GetRegister()->GetViewPort();

		if (m_ViewPort->IsHovered())
		{
			auto pair = m_ViewPort->GetMousePosInViewport();

			VulkanRenderBackend::GetRendererResourcePool()->AccessRowResource("ID")->CopyImageTexelToBuffer(pair.first, pair.second, (void*)&m_WorldPickID[0]);
		}
	}
}