/**
* @file ImguiGizmos.cpp.
* @brief The ImguiGizmos Class Implementation.
* @author Spices.
*/

#include "Pchheader.h"
#include "ImguiGBufferVisualizer.h"

#include "Render/Vulkan/VulkanRenderBackend.h"

namespace Spices {

    ImguiGBufferVisualizer::ImguiGBufferVisualizer(
        const std::string& panelName , 
        FrameInfo&         frameInfo
    )
        : ImguiSlate(panelName, frameInfo)
    {
        SPICES_PROFILE_ZONE;

        QueryGBufferID();
    }

    void ImguiGBufferVisualizer::OnRender()
    {
        SPICES_PROFILE_ZONE;

        if (!m_IsSlateOn) return;

        /**
        * @brief Begin render GBuffer Visualizer.
        */
        Begin("GBuffer");

        ImVec2 size = { m_Width , m_Height };

        /**
        * @brief Render SceneColor.
        */
        {
            SPICES_PROFILE_ZONEN("Render SceneColor");

            ImGui::Text("SceneColor");
            ImGui::Image(m_GBufferID.SceneColorID, size);
            ImGui::Separator();
        }

        /**
        * @brief Render Diffuse.
        */
        {
            SPICES_PROFILE_ZONEN("Render Diffuse");

            ImGui::Text("Diffuse");
            ImGui::Image(m_GBufferID.DiffuseID, size);
            ImGui::Separator();
        }

        /**
        * @brief Render Normal.
        */
        {
            SPICES_PROFILE_ZONEN("Render Normal");

            ImGui::Text("Normal");
            ImGui::Image(m_GBufferID.NormalID, size);
            ImGui::Separator();
        }

        /**
        * @brief Render Specular.
        */
        {
            SPICES_PROFILE_ZONEN("Render Specular");

            ImGui::Text("Specular");
            ImGui::Image(m_GBufferID.SpecularID, size);
            ImGui::Separator();
        }

        /**
        * @brief End render GBuffer Visualizer.
        */
        End();

    }

    void ImguiGBufferVisualizer::OnEvent(Event& event)
    {
        SPICES_PROFILE_ZONE;

        /**
        * @brief Instance a EventDispatcher.
        */
        EventDispatcher dispatcher(event);

        /**
        * @brief Dispatch SlateResizeEvent to ImguiGBufferVisualizer::OnSlateResized.
        */
        dispatcher.Dispatch<SlateResizeEvent>(BIND_EVENT_FN(ImguiGBufferVisualizer::OnSlateResized));
    }

    bool ImguiGBufferVisualizer::OnSlateResized(SlateResizeEvent& event)
    {
        SPICES_PROFILE_ZONE;

        /**
        * @brief Free old DescriptorSet.
        */
        m_GBufferID.Free();

        m_Width = 500;
        m_Height = m_Width * event.GetHeight() / static_cast<float>(event.GetWidth());

        /**
        * @rief Re Query GBufferID.
        */
        QueryGBufferID();

        /**
        * @brief Do not block the event.
        */
        return false;
    }

    void ImguiGBufferVisualizer::QueryGBufferID()
    {
        SPICES_PROFILE_ZONE;

        VkDescriptorImageInfo* sceneColorInfo = VulkanRenderBackend::GetRendererResourcePool()->AccessResource({ "SceneColor" });
        VkDescriptorImageInfo* diffuseInfo = VulkanRenderBackend::GetRendererResourcePool()->AccessResource({ "Diffuse" });
        VkDescriptorImageInfo* normalInfo = VulkanRenderBackend::GetRendererResourcePool()->AccessResource({ "Normal" });
        VkDescriptorImageInfo* specularInfo = VulkanRenderBackend::GetRendererResourcePool()->AccessResource({ "Specular" });

        m_GBufferID.SceneColorID  = ImGui_ImplVulkan_AddTexture(sceneColorInfo->sampler , sceneColorInfo->imageView , sceneColorInfo->imageLayout  );
        m_GBufferID.DiffuseID     = ImGui_ImplVulkan_AddTexture(diffuseInfo->sampler    , diffuseInfo->imageView   ,  diffuseInfo->imageLayout     );
        m_GBufferID.NormalID      = ImGui_ImplVulkan_AddTexture(normalInfo->sampler     , normalInfo->imageView     , normalInfo->imageLayout      );
        m_GBufferID.SpecularID    = ImGui_ImplVulkan_AddTexture(specularInfo->sampler   , specularInfo->imageView   , specularInfo->imageLayout    );
    }
}