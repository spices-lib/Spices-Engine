/**
* @file ImguiViewport.cpp.
* @brief The ImguiViewport Class Implementation.
* @author Spices.
*/

#include "Pchheader.h"
#include "ImguiViewport.h"

#include "Render/Vulkan/VulkanRenderBackend.h"
#include "Systems/SlateSystem.h"
#include "ImguiFloatingInfo.h"
#include "ImGuizmo.h"
#include "ImguiViewportToolBar.h"

namespace Spices {

    ImguiViewport::ImguiViewport(
        const std::string& panelName , 
        FrameInfo&         frameInfo ,
        uint32_t           index
    )
        : ImguiSlate(panelName, frameInfo)
        , m_Index(index)
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
            m_ViewportID = reinterpret_cast<ImTextureID>(ImGui_ImplVulkan_AddTexture(info->sampler, info->imageView, info->imageLayout));
        }

        /**
        * @brief Instance a FloattingInfo.
        */
        {
            std::stringstream ss;
            ss << "FloatingInfo_" << m_Index;

            m_FloatingInfo = SlateSystem::GetRegister()->Register<ImguiFloatingInfo>(false, ss.str(), this);
        }

        /**
        * @brief Instance a Gizmos.
        */
        {
            std::stringstream ss;
            ss << "Gizmos_" << m_Index;

            m_Gizmos = SlateSystem::GetRegister()->Register<ImguiGizmos>(false, ss.str(), this);
        }

        /**
        * @brief Instance a Gizmos.
        */
        {
            std::stringstream ss;
            ss << "ToolBar_" << m_Index;

            m_ToolBar = SlateSystem::GetRegister()->Register<ImguiViewportToolBar>(false, ss.str(), this);
        }

        /**
        * @brief Build Toggle behave list.
        */
        {
            m_ToggleStateList = std::make_shared<scl::behave_state_list<void>>();

            {
                auto state = m_ToggleStateList->AddNode();
                state->PushBehave("Toggle", [&]() {

                    m_IsToggled = !m_IsToggled;

                    if (m_IsToggled)
                    {
                        m_CachedPanelPos = m_PanelPos;
                        m_CachedPanelSize = m_PanelSize;

                        const ImGuiViewport* viewport = ImGui::GetMainViewport();
                        ImGui::SetNextWindowPos(viewport->Pos);
                        ImGui::SetNextWindowSize(viewport->Size);
                    }
                    else
                    {
                        ImGui::SetNextWindowPos(m_CachedPanelPos);
                        ImGui::SetNextWindowSize(m_CachedPanelSize);

                        ImGuiID dockspaceID = ImGui::GetID("DockSpace");
                        ImGui::DockSpace(dockspaceID, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_PassthruCentralNode);
                    }
                });
            }
            {
                auto state = m_ToggleStateList->AddNode();
                state->PushBehave("Toggle", [&]() {

                    if (m_IsToggled)
                    {
                        const ImGuiViewport* viewport = ImGui::GetMainViewport();
                        ImGui::SetNextWindowPos(viewport->Pos);
                        ImGui::SetNextWindowSize(viewport->Size);
                    }
                });
            }

            m_ToggleStateList->SetState(1);
        }
    }

    void ImguiViewport::OnRender()
    {
        SPICES_PROFILE_ZONE;

        m_ToggleStateList->GetState()->ExecuteBehave("Toggle");

        /**
        * @brief Begin render Console.
        */
        Begin(0.5, ImGuiWindowFlags_NoTitleBar);

        /**
        * @brief Render Viewport image.
        */
        {
            SPICES_PROFILE_ZONEN("Render Viewport Image");

            ImGui::Image(m_ViewportID, m_PanelSize);

            //ImVec2 p0 = ImGui::GetItemRectMin();
            //ImVec2 p1 = ImGui::GetItemRectMax();
            //
            //
            //ImDrawList* draw_list = ImGui::GetWindowDrawList();
            //draw_list->PushTextureID(m_ViewportID);
            //draw_list->PrimReserve(6, 4);
            //draw_list->PrimRectUV(p0, p1, ImVec2(0.0f, 0.0f), ImVec2(1.0f, 1.0f), ImGui::GetColorU32(ImVec4(1.0f, 1.0f, 1.0f, 1.0f)));
            //draw_list->AddCallback([](const ImDrawList* drawList, const ImDrawCmd* cmd) {

            //    std::cout << cmd->GetTexID() << std::endl;

            //    //vkCmdBindDescriptorSets(command_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, bd->PipelineLayout, 0, 1, desc_set, 0, nullptr);
            //    //vkCmdDrawIndexed(command_buffer, cmd->ElemCount, 1, cmd->IdxOffset + global_idx_offset, cmd->VtxOffset + global_vtx_offset, 0);

            //}, nullptr);
            //draw_list->PopTextureID();
            //draw_list->AddCallback(ImDrawCallback_ResetRenderState, nullptr);


        }

        /**
        * @brief Render ToolBar.
        */
        m_ToolBar->OnRender();

        /**
        * @brief Render FloatingInfo.
        */
        m_FloatingInfo->OnRender();

        /**
        * @brief Render Gizmos.
        */
        m_Gizmos->OnRender();

        /**
        * @brief End Viewport Slate.
        */
        End();

        m_ToggleStateList->IncreateState();
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
        dispatcher.Dispatch<KeyPressedEvent>(BIND_EVENT_FN(ImguiViewport::OnToggleSlate));
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
        ImGui_ImplVulkan_RemoveTexture(reinterpret_cast<VkDescriptorSet>(m_ViewportID));

        /**
        * @brief Get SceneColor Info again.
        */
        VkDescriptorImageInfo* info = VulkanRenderBackend::GetRendererResourcePool()->AccessResource({ "SceneColor" });

        /**
        * @brief Create SceneColor DescriptorSet.
        */
        m_ViewportID = reinterpret_cast<ImTextureID>(ImGui_ImplVulkan_AddTexture(info->sampler, info->imageView, info->imageLayout));

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
        ImGui_ImplVulkan_RemoveTexture(reinterpret_cast<VkDescriptorSet>(m_ViewportID));

        /**
        * @brief Get SceneColor Info again.
        */
        VkDescriptorImageInfo* info = VulkanRenderBackend::GetRendererResourcePool()->AccessResource({ "SceneColor" });

        /**
        * @brief Create SceneColor DescriptorSet.
        */
        m_ViewportID = reinterpret_cast<ImTextureID>(ImGui_ImplVulkan_AddTexture(info->sampler, info->imageView, info->imageLayout));

        /**
        * @brief Do not block the event.
        */
        return false;
    }

    bool ImguiViewport::OnToggleSlate(KeyPressedEvent& event)
	{
		SPICES_PROFILE_ZONE;

		if (event.GetKeyCode() == Key::F11)
		{
            m_ToggleStateList->ResetState();
		}

		return false;
	}
}