/**
* @file ImguiViewport.cpp.
* @brief The ImguiViewport Class Implementation.
* @author Spices.
*/

#include "Pchheader.h"
#include "ImguiViewport.h"

#include "Render/Vulkan/VulkanRenderBackend.h"
#include "Systems/SlateSystem.h"
#include "ImguiFloattingInfo.h"
#include "ImGuizmo.h"
#include "ImguiViewportToolBar.h"

namespace Spices {

    ImguiViewport::ImguiViewport(
        const std::string& panelName , 
        FrameInfo&         frameInfo
    )
        : ImguiSlate(panelName, frameInfo)
    {
        SPICES_PROFILE_ZONE;

        {
            SPICES_PROFILE_ZONEN("Create SceneColor ImTextureID");

            /**
            * @brief Get SceneColor Info.
            */
            VkDescriptorImageInfo* info = VulkanRenderBackend::GetRendererResourcePool()->AccessResource({ "SceneColor" });

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

        /**
        * @brief Instance a Gizmos.
        */
        m_ToolBar = SlateSystem::GetRegister()->Register<ImguiViewportToolBar>(false, "ToolBar", this);

    }

    void ImguiViewport::OnRender()
    {
        SPICES_PROFILE_ZONE;

        /**
        * @brief Begin render Console.
        */
        Begin(0.5, ImGuiWindowFlags_NoTitleBar);

        /**
        * @brief Render Viewport image.
        */
        {
            SPICES_PROFILE_ZONEN("Render Viewport Image");

            ImGui::Image((void*)m_ViewportID, m_PanelSize);
        }

        /**
        * @brief Render ToolBar.
        */
        m_ToolBar->OnRender();

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
        SPICES_PROFILE_ZONE;

        /**
        * @brief Instance a EventDispatcher.
        */
        EventDispatcher dispatcher(event);

        /**
        * @brief Dispatch SlateResizeEvent.
        */
        dispatcher.Dispatch<SlateResizeEvent>(BIND_EVENT_FN(ImguiViewport::OnSlateResize));
        dispatcher.Dispatch<WindowResizeOverEvent>(BIND_EVENT_FN(ImguiViewport::OnWindowResizeOver));
    }

    void ImguiViewport::QueryIsResizedThisFrame(const ImVec2& thisFrameSize)
    {
        SPICES_PROFILE_ZONE;

        /**
        * @brief Clamp min value to 1 for viewport.
        */
        ImVec2 tempSize = ImVec2(glm::max(thisFrameSize.x, 1.0f), glm::max(thisFrameSize.y, 1.0f));

        if (m_PanelSize.x != tempSize.x || m_PanelSize.y != tempSize.y)
        {
            m_IsResized = true;
        }
        else
        {
            m_IsResized = false;
        }

        m_PanelSize = tempSize;
    }

    std::pair<uint32_t, uint32_t> ImguiViewport::GetMousePosInViewport()
    {
        SPICES_PROFILE_ZONE;

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
        SPICES_PROFILE_ZONE;

        /**
        * @brief Free old Viewport image DescriptorSet.
        */
        ImGui_ImplVulkan_RemoveTexture(static_cast<VkDescriptorSet>(m_ViewportID));

        /**
        * @brief Get SceneColor Info again.
        */
        VkDescriptorImageInfo* info = VulkanRenderBackend::GetRendererResourcePool()->AccessResource({ "SceneColor" });

        /**
        * @brief Create SceneColor DescriptorSet.
        */
        m_ViewportID = ImGui_ImplVulkan_AddTexture(info->sampler, info->imageView, info->imageLayout);

        /**
        * @brief Do not block the event.
        */
        return false;
    }

    bool ImguiViewport::OnWindowResizeOver(WindowResizeOverEvent& event)
    {
        SPICES_PROFILE_ZONE;

        /**
        * @brief Free old Viewport image DescriptorSet.
        */
        ImGui_ImplVulkan_RemoveTexture(static_cast<VkDescriptorSet>(m_ViewportID));

        /**
        * @brief Get SceneColor Info again.
        */
        VkDescriptorImageInfo* info = VulkanRenderBackend::GetRendererResourcePool()->AccessResource({ "SceneColor" });

        /**
        * @brief Create SceneColor DescriptorSet.
        */
        m_ViewportID = ImGui_ImplVulkan_AddTexture(info->sampler, info->imageView, info->imageLayout);

        /**
        * @brief Do not block the event.
        */
        return false;
    }
}