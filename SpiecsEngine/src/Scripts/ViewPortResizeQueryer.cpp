#include "Pchheader.h"
#include "ViewPortResizeQueryer.h"
#include "Systems/SlateSystem.h"
#include "Slate/Imgui/ViewPort/ImguiViewport.h"
#include "Core/Event/SlateEvent.h"
#include "Render/Vulkan/VulkanRenderBackend.h"

namespace Spiecs {

	void ViewPortResizeQueryer::OnTick(TimeStep& ts)
	{
		/**
		* @brief The first frame, we will not get register pointer.
		*/
		if (!SlateSystem::GetRegister()) return;

		if (!m_ViewPort) m_ViewPort = SlateSystem::GetRegister()->GetViewPort();

		if (m_ViewPort->IsResizedThisFrame())
		{
			vkDeviceWaitIdle(VulkanRenderBackend::GetState().m_Device);

			SlateResizeEvent event(m_ViewPort->GetPanelSize().x, m_ViewPort->GetPanelSize().y);

			Event::GetEventCallbackFn()(event);
		}
	}
}