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
       
        m_PanelSize = ImGui::GetContentRegionAvail();
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

        dispatcher.Dispatch<WindowOnResizedEvent>(BIND_EVENT_FN(ImguiViewport::OnWindowResized));
    }

    bool ImguiViewport::OnWindowResized(WindowOnResizedEvent& event)
    {
        VkDescriptorImageInfo* info = VulkanRenderBackend::GetRendererResourcePool()->AccessResource("FinalColor");

        m_ViewportID = ImGui_ImplVulkan_AddTexture(info->sampler, info->imageView, info->imageLayout);

        return false;
    }
}