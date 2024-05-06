#include "Pchheader.h"
#include "ImguiGBufferVisualizer.h"
#include "Render/Vulkan/VulkanRenderBackend.h"

namespace Spiecs {

    ImguiGBufferVisualizer::ImguiGBufferVisualizer(const std::string& panelName, FrameInfo& frameInfo)
        : ImguiSlate(panelName, frameInfo)
    {
        QueryGBufferID();
    }

    void ImguiGBufferVisualizer::OnRender()
    {
        if (!m_IsSlateOn) return;

        Begin("GBuffer");

        ImVec2 size = { m_Width , m_Height };

        ImGui::Text("FinalColor");
        ImGui::Image(m_GBufferID.FinalColorID, size);
        ImGui::Separator;

        ImGui::Text("BaseColor");
        ImGui::Image(m_GBufferID.BaseColorID, size);
        ImGui::Separator;

        ImGui::Text("Normal");
        ImGui::Image(m_GBufferID.NormalID, size);
        ImGui::Separator;

        End();

    }

    void ImguiGBufferVisualizer::OnEvent(Event& event)
    {
        EventDispatcher dispatcher(event);
        dispatcher.Dispatch<SlateResizeEvent>(BIND_EVENT_FN(ImguiGBufferVisualizer::OnSlateResized));
    }

    bool ImguiGBufferVisualizer::OnSlateResized(SlateResizeEvent& event)
    {
        m_GBufferID.Free();

        m_Width = 500;
        m_Height = m_Width * event.GetHeight() / static_cast<float>(event.GetWidth());

        QueryGBufferID();
        return false;
    }

    void ImguiGBufferVisualizer::QueryGBufferID()
    {
        VkDescriptorImageInfo* finalColorInfo = VulkanRenderBackend::GetRendererResourcePool()->AccessResource("FinalColor");
        VkDescriptorImageInfo* basecClorInfo  = VulkanRenderBackend::GetRendererResourcePool()->AccessResource("BaseColor" );
        VkDescriptorImageInfo* normalInfo     = VulkanRenderBackend::GetRendererResourcePool()->AccessResource("Normal"    );

        m_GBufferID.FinalColorID = ImGui_ImplVulkan_AddTexture(finalColorInfo->sampler, finalColorInfo->imageView, finalColorInfo->imageLayout);
        m_GBufferID.BaseColorID  = ImGui_ImplVulkan_AddTexture(basecClorInfo->sampler , basecClorInfo->imageView , basecClorInfo->imageLayout );
        m_GBufferID.NormalID     = ImGui_ImplVulkan_AddTexture(normalInfo->sampler    , normalInfo->imageView    , normalInfo->imageLayout    );
    }
}