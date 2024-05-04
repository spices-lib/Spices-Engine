#include "Pchheader.h"
#include "ImguiGBufferVisualizer.h"
#include "Render/Vulkan/VulkanRenderBackend.h"

namespace Spiecs {

    ImguiGBufferVisualizer::ImguiGBufferVisualizer(const std::string& panelName)
        : ImguiSlate(panelName)
    {
        QueryGBufferID();
    }

    void ImguiGBufferVisualizer::OnRender()
    {
        if (!m_IsSlateOn) return;

        Begin("GBuffer");

        ImGui::Text("FinalColor");
        ImGui::Image(m_GBufferID.FinalColorID, ImVec2(500, 500));
        ImGui::Separator;

        ImGui::Text("BaseColor");
        ImGui::Image(m_GBufferID.BaseColorID, ImVec2(500, 500));
        ImGui::Separator;

        ImGui::Text("Normal");
        ImGui::Image(m_GBufferID.NormalID, ImVec2(500, 500));
        ImGui::Separator;

        End();

    }

    void ImguiGBufferVisualizer::OnEvent(Event& event)
    {
        EventDispatcher dispatcher(event);
        dispatcher.Dispatch<WindowResizeOverEvent>(BIND_EVENT_FN(ImguiGBufferVisualizer::OnWindowResized));
    }

    bool ImguiGBufferVisualizer::OnWindowResized(WindowResizeOverEvent& event)
    {
        m_GBufferID.Free();

        QueryGBufferID();
        return false;
    }

    void ImguiGBufferVisualizer::QueryGBufferID()
    {
        VkDescriptorImageInfo* finalColorInfo = VulkanRenderBackend::GetRendererResourcePool()->AccessResource("FinalColor");
        VkDescriptorImageInfo* basecClorInfo = VulkanRenderBackend::GetRendererResourcePool()->AccessResource("BaseColor");
        VkDescriptorImageInfo* normalInfo = VulkanRenderBackend::GetRendererResourcePool()->AccessResource("Normal");

        m_GBufferID.FinalColorID = ImGui_ImplVulkan_AddTexture(finalColorInfo->sampler, finalColorInfo->imageView, finalColorInfo->imageLayout);
        m_GBufferID.BaseColorID = ImGui_ImplVulkan_AddTexture(basecClorInfo->sampler, basecClorInfo->imageView, basecClorInfo->imageLayout);
        m_GBufferID.NormalID = ImGui_ImplVulkan_AddTexture(normalInfo->sampler, normalInfo->imageView, normalInfo->imageLayout);
    }
}