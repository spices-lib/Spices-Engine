#include "Pchheader.h"
#include "ImguiViewport.h"
#include "Render/Vulkan/VulkanRenderBackend.h"

namespace Spiecs {

    ImguiViewport::ImguiViewport()
    {
        VkDescriptorImageInfo* info = VulkanRenderBackend::GetRendererResourcePool()->AccessResource("FinalColor");

        m_ViewportID = ImGui_ImplVulkan_AddTexture(info->sampler, info->imageView, info->imageLayout);
    }

    void ImguiViewport::OnRender()
    {

        ImGui::Begin("Viewport");
        ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
        ImGui::Image(m_ViewportID, ImVec2{ viewportPanelSize.x, viewportPanelSize.y });
        ImGui::End();
    }
}