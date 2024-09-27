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
        Begin(m_PanelName.c_str());

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
        * @brief Render Albedo.
        */
        {
            SPICES_PROFILE_ZONEN("Render Albedo");

            ImGui::Text("Albedo");
            ImGui::Image(m_GBufferID.AlbedoID, size);
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
        * @brief Render Roughness.
        */
        {
            SPICES_PROFILE_ZONEN("Render Roughness");

            ImGui::Text("Roughness");
            ImGui::Image(m_GBufferID.RoughnessID, size);
            ImGui::Separator();
        }

        /**
        * @brief Render Metallic.
        */
        {
            SPICES_PROFILE_ZONEN("Render Metallic");

            ImGui::Text("Metallic");
            ImGui::Image(m_GBufferID.MetallicID, size);
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
        VkDescriptorImageInfo* albedoInfo     = VulkanRenderBackend::GetRendererResourcePool()->AccessResource({ "Albedo"     });
        VkDescriptorImageInfo* normalInfo     = VulkanRenderBackend::GetRendererResourcePool()->AccessResource({ "Normal"     });
        VkDescriptorImageInfo* roughnessInfo  = VulkanRenderBackend::GetRendererResourcePool()->AccessResource({ "Roughness"  });
        VkDescriptorImageInfo* metallicInfo   = VulkanRenderBackend::GetRendererResourcePool()->AccessResource({ "Metallic"   });

        m_GBufferID.SceneColorID  = ImGui_ImplVulkan_AddTexture(sceneColorInfo->sampler , sceneColorInfo->imageView , sceneColorInfo->imageLayout  );
        m_GBufferID.AlbedoID      = ImGui_ImplVulkan_AddTexture(albedoInfo->sampler     , albedoInfo->imageView     , albedoInfo->imageLayout      );
        m_GBufferID.NormalID      = ImGui_ImplVulkan_AddTexture(normalInfo->sampler     , normalInfo->imageView     , normalInfo->imageLayout      );
        m_GBufferID.RoughnessID   = ImGui_ImplVulkan_AddTexture(roughnessInfo->sampler  , roughnessInfo->imageView  , roughnessInfo->imageLayout   );
        m_GBufferID.MetallicID    = ImGui_ImplVulkan_AddTexture(metallicInfo->sampler   , metallicInfo->imageView   , metallicInfo->imageLayout    );
    }
}