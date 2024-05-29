/**
* @file ImguiViewport.cpp.
* @brief The ImguiViewport Class Implementation.
* @author Spiecs.
*/

#include "Pchheader.h"
#include "ImguiViewport.h"

#include "Render/Vulkan/VulkanRenderBackend.h"
#include "Systems/SlateSystem.h"
#include "ImguiFloattingInfo.h"
#include "ImGuizmo.h"

namespace Spiecs {

    ImguiViewport::ImguiViewport(
        const std::string& panelName , 
        FrameInfo&         frameInfo
    )
        : ImguiSlate(panelName, frameInfo)
    {
        ZoneScoped;

        {
            ZoneScopedN("Create SceneColor ImTextureID");

            /**
            * @brief Get SceneColor Info.
            */
            VkDescriptorImageInfo* info = VulkanRenderBackend::GetRendererResourcePool()->AccessResource("SceneColor");

            /**
            * @brief Create SceneColor DescriptorSet.
            */
            m_ViewportID = ImGui_ImplVulkan_AddTexture(info->sampler, info->imageView, info->imageLayout);
        }

        /**
        * @brief Instance a FloattingInfo.
        */
        m_FloattingInfo = SlateSystem::GetRegister()->Register<ImguiFloattingInfo>(false, "FloattingInfo", this);


        /**
        * @brief Instance a Gizmos.
        */
        m_Gizmos = SlateSystem::GetRegister()->Register<ImguiGizmos>(false, "Gizmos", this);

    }

    void ImguiViewport::OnRender()
    {
        ZoneScoped;

        /**
        * @brief Begin the viewport as a Main Docking Space.
        * @todo Multiple Viewport Support.
        */
        BeginMainDock();

        /**
        * @brief Query Resize Event Condition.
        */
        QueryIsResizedThisFrame(ImGui::GetContentRegionAvail());

        /**
        * @brief Query Slate State, maybe implementate in parent.
        */
        {
            ZoneScopedN("Query Slate State");

            m_PanelPos = ImGui::GetWindowPos();
            m_IsFocused = ImGui::IsWindowFocused();
            m_IsHovered = ImGui::IsWindowHovered();
        }

        /**
        * @brief Render Viewport image.
        */
        {
            ZoneScopedN("Render Viewport Image");

            ImGui::Image((void*)m_ViewportID, m_PanelSize);
        }

        /**
        * @brief Render FloatingInfo.
        */
        m_FloattingInfo->OnRender();

        /**
        * @brief Render Gizmos.
        */
        m_Gizmos->OnRender();

        /**
        * @brief End Viewport Slate.
        */
        End();
    }

    void ImguiViewport::OnEvent(Event& event)
    {
        ZoneScoped;

        /**
        * @brief Instance a EventDispatcher.
        */
        EventDispatcher dispatcher(event);

        /**
        * @brief Dispatch SlateResizeEvent to ImguiViewport::OnSlateResize.
        */
        dispatcher.Dispatch<SlateResizeEvent>(BIND_EVENT_FN(ImguiViewport::OnSlateResize));
    }

    std::pair<uint32_t, uint32_t> ImguiViewport::GetMousePosInViewport()
    {
        ZoneScoped;

        ImGuiIO& io = ImGui::GetIO();
        ImVec2 viewportPos = io.MousePos - m_PanelPos;

        std::pair<uint32_t, uint32_t> pair = std::make_pair(
            static_cast<uint32_t>(viewportPos.x), 
            static_cast<uint32_t>(viewportPos.y)
        );

        return pair;
    }

    bool ImguiViewport::OnSlateResize(SlateResizeEvent& event)
    {
        ZoneScoped;

        /**
        * @brief Free old Viewport image DescriptorSet.
        */
        ImGui_ImplVulkan_RemoveTexture(static_cast<VkDescriptorSet>(m_ViewportID));

        /**
        * @brief Get SceneColor Info again.
        */
        VkDescriptorImageInfo* info = VulkanRenderBackend::GetRendererResourcePool()->AccessResource("SceneColor");

        /**
        * @brief Create SceneColor DescriptorSet.
        */
        m_ViewportID = ImGui_ImplVulkan_AddTexture(info->sampler, info->imageView, info->imageLayout);

        /**
        * @brief Do not block the event.
        */
        return false;
    }

    void ImguiViewport::QueryIsResizedThisFrame(ImVec2 thisFrameSize)
    {
        ZoneScoped;

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