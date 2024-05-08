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

        ImGui::Text("SceneColor");
        ImGui::Image(m_GBufferID.SceneColorID, size);
        ImGui::Separator;

        ImGui::Text("Diffuse");
        ImGui::Image(m_GBufferID.DiffuseID, size);
        ImGui::Separator;

        ImGui::Text("Normal");
        ImGui::Image(m_GBufferID.NormalID, size);
        ImGui::Separator;

        ImGui::Text("Specular");
        ImGui::Image(m_GBufferID.SpecularID, size);
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
        VkDescriptorImageInfo* sceneColorInfo = VulkanRenderBackend::GetRendererResourcePool()->AccessResource("SceneColor");
        VkDescriptorImageInfo* diffuseInfo  = VulkanRenderBackend::GetRendererResourcePool()->AccessResource("Diffuse" );
        VkDescriptorImageInfo* normalInfo     = VulkanRenderBackend::GetRendererResourcePool()->AccessResource("Normal"    );
        VkDescriptorImageInfo* specularInfo     = VulkanRenderBackend::GetRendererResourcePool()->AccessResource("Specular"    );

        m_GBufferID.SceneColorID = ImGui_ImplVulkan_AddTexture(sceneColorInfo->sampler , sceneColorInfo->imageView , sceneColorInfo->imageLayout  );
        m_GBufferID.DiffuseID     = ImGui_ImplVulkan_AddTexture(diffuseInfo->sampler     , diffuseInfo->imageView     , diffuseInfo->imageLayout      );
        m_GBufferID.NormalID     = ImGui_ImplVulkan_AddTexture(normalInfo->sampler     , normalInfo->imageView     , normalInfo->imageLayout      );
        m_GBufferID.SpecularID   = ImGui_ImplVulkan_AddTexture(specularInfo->sampler   , specularInfo->imageView   , specularInfo->imageLayout    );
    }
}