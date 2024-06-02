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
#include "ImguiViewportToolBar.h"

namespace Spiecs {

    ImguiViewport::ImguiViewport(
        const std::string& panelName , 
        FrameInfo&         frameInfo
    )
        : ImguiSlate(panelName, frameInfo)
    {
        SPIECS_PROFILE_ZONE;

        {
            SPIECS_PROFILE_ZONEN("Create SceneColor ImTextureID");

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

        /**
        * @brief Instance a Gizmos.
        */
        m_ToolBar = SlateSystem::GetRegister()->Register<ImguiViewportToolBar>(false, "ToolBar", this);

    }

    void ImguiViewport::OnRender()
    {
        SPIECS_PROFILE_ZONE;

        /**
        * @brief Begin render Console.
        */
        Begin(0.5, ImGuiWindowFlags_NoTitleBar);

        /**
        * @brief Render Viewport image.
        */
        {
            SPIECS_PROFILE_ZONEN("Render Viewport Image");

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
        SPIECS_PROFILE_ZONE;

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
        SPIECS_PROFILE_ZONE;

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
        SPIECS_PROFILE_ZONE;

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
}