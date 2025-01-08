/**
* @file ViewPortResizeQuerier.cpp.
* @brief The ViewPortResizeQuerier & NativeScriptRegister Class Implementation.
* @author Spices.
*/

#include "Pchheader.h"
#include "ViewPortResizeQuerier.h"
#include "Systems/SlateSystem.h"
#include "Slate/Imgui/ViewPort/ImguiViewport.h"
#include "Core/Event/SlateEvent.h"
#include "Render/Vulkan/VulkanRenderBackend.h"

namespace Spices {

	void ViewPortResizeQuerier::OnTick(TimeStep& ts)
	{
		SPICES_PROFILE_ZONE;
		
		/**
		* @brief The first frame, we will not get register pointer.
		*/
		if (!SlateSystem::GetRegister()) return;

		if (!m_ViewPort.lock()) m_ViewPort = SlateSystem::GetRegister()->GetViewPort();

		if (m_ViewPort.lock()->IsResizedThisFrame())
		{
			/**
			* @brief Might not needed?
			*/
			VK_CHECK(vkQueueWaitIdle(VulkanRenderBackend::GetState().m_GraphicQueue))
				
			SlateResizeEvent event(static_cast<uint32_t>(m_ViewPort.lock()->GetPanelSize().x), static_cast<uint32_t>(m_ViewPort.lock()->GetPanelSize().y));

			Event::GetEventCallbackFn()(event);
		}
	}
}