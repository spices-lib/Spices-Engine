/**
* @file ImguiVirtualGeometryVisualizer.cpp.
* @brief The ImguiVirtualGeometryVisualizer Class Implementation.
* @author Spices.
*/

#include "Pchheader.h"
#include "ImguiVirtualGeometryVisualizer.h"

#include "Render/Vulkan/VulkanRenderBackend.h"

namespace Spices {

    ImguiVirtualGeometryVisualizer::ImguiVirtualGeometryVisualizer(
        const std::string& panelName,
        FrameInfo& frameInfo
    )
        : ImguiSlate(panelName, frameInfo)
    {
        SPICES_PROFILE_ZONE;

        QueryID();
    }

    void ImguiVirtualGeometryVisualizer::OnRender()
    {
        SPICES_PROFILE_ZONE;

        if (!m_IsSlateOn) return;

        /**
        * @brief Begin render GBuffer Visualizer.
        */
        Begin("GBuffer");

        ImVec2 size = { m_Width , m_Height };

        /**
        * @brief Render TriangleID.
        */
        {
            SPICES_PROFILE_ZONEN("Render TriangleID");

            ImGui::Text("TriangleID");
            ImGui::Image(m_BufferID.TriangleID, size);
            ImGui::Separator();
        }

        /**
        * @brief Render MeshletID.
        */
        {
            SPICES_PROFILE_ZONEN("Render MeshletID");

            ImGui::Text("MeshletID");
            ImGui::Image(m_BufferID.MeshletID, size);
            ImGui::Separator();
        }

        /**
        * @brief End render GBuffer Visualizer.
        */
        End();

    }

    void ImguiVirtualGeometryVisualizer::OnEvent(Event& event)
    {
        SPICES_PROFILE_ZONE;

        /**
        * @brief Instance a EventDispatcher.
        */
        EventDispatcher dispatcher(event);

        /**
        * @brief Dispatch SlateResizeEvent to ImguiGBufferVisualizer::OnSlateResized.
        */
        dispatcher.Dispatch<SlateResizeEvent>(BIND_EVENT_FN(ImguiVirtualGeometryVisualizer::OnSlateResized));
    }

    bool ImguiVirtualGeometryVisualizer::OnSlateResized(SlateResizeEvent& event)
    {
        SPICES_PROFILE_ZONE;

        /**
        * @brief Free old DescriptorSet.
        */
        m_BufferID.Free();

        m_Width = 500;
        m_Height = m_Width * event.GetHeight() / static_cast<float>(event.GetWidth());

        /**
        * @rief Re Query ID.
        */
        QueryID();

        /**
        * @brief Do not block the event.
        */
        return false;
    }

    void ImguiVirtualGeometryVisualizer::QueryID()
    {
        SPICES_PROFILE_ZONE;

        VkDescriptorImageInfo* triangleID = VulkanRenderBackend::GetRendererResourcePool()->AccessResource({ "TriangleID" });
        VkDescriptorImageInfo* meshletID = VulkanRenderBackend::GetRendererResourcePool()->AccessResource({ "MeshletID" });

        m_BufferID.TriangleID = ImGui_ImplVulkan_AddTexture(triangleID->sampler, triangleID->imageView, triangleID->imageLayout);
        m_BufferID.MeshletID = ImGui_ImplVulkan_AddTexture(meshletID->sampler, meshletID->imageView, meshletID->imageLayout);
    }
}