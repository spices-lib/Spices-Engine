#include "Pchheader.h"
#include "ImguiViewport.h"
#include "Render/Vulkan/VulkanRenderBackend.h"
#include "Systems/SlateSystem.h"
#include "ImguiFloattingInfo.h"

namespace Spiecs {

    ImguiViewport::ImguiViewport(const std::string& panelName)
        : ImguiSlate(panelName)
    {
        VkDescriptorImageInfo* info = VulkanRenderBackend::GetRendererResourcePool()->AccessResource("FinalColor");

        m_ViewportID = ImGui_ImplVulkan_AddTexture(info->sampler, info->imageView, info->imageLayout);

        m_FloattingInfo = SlateSystem::GetRegister()->Register<ImguiFloattingInfo>(false, "FloattingInfo", this);
    }

    void ImguiViewport::OnRender()
    {
        BeginMainDock();

        QueryIsResizedThisFrame(ImGui::GetContentRegionAvail());
        m_PanelPos = ImGui::GetWindowPos();
           
        m_IsFocused = ImGui::IsWindowFocused();
        m_IsHovered = ImGui::IsWindowHovered();

        m_FloattingInfo->OnRender();

        ImGui::Image((void*)m_ViewportID, m_PanelSize);
        End();
    }

    void ImguiViewport::OnEvent(Event& event)
    {
        EventDispatcher dispatcher(event);

        dispatcher.Dispatch<SlateResizeEvent>(BIND_EVENT_FN(ImguiViewport::OnSlateResize));
    }

    bool ImguiViewport::OnSlateResize(SlateResizeEvent& event)
    {
        ImGui_ImplVulkan_RemoveTexture(static_cast<VkDescriptorSet>(m_ViewportID));

        VkDescriptorImageInfo* info = VulkanRenderBackend::GetRendererResourcePool()->AccessResource("FinalColor");

        m_ViewportID = ImGui_ImplVulkan_AddTexture(info->sampler, info->imageView, info->imageLayout);

        return false;
    }

    void ImguiViewport::QueryIsResizedThisFrame(ImVec2 thisFrameSize)
    {
        if (m_PanelSize.x != thisFrameSize.x || m_PanelSize.y != thisFrameSize.y)
        {
            isResized = true;
        }
        else
        {
            isResized = false;
        }
        m_PanelSize = thisFrameSize;
    }
}