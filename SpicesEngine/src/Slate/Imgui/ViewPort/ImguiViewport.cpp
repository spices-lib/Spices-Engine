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
#include "..\..\..\Core\Thread\ThreadModel.h"
#include "Slate/SlateStyleLayout.h"

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

            m_ViewportContext = std::make_shared<SlateImage>("SceneColor", "SlateRenderer.Slate.Default");
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
                        AsyncTask(ThreadPoolEnum::Game, []() {
                            SlateStyleLayout::Get()->StoreLayoutCache();
                        });
                    }
                    else
                    {
                        m_WindowFlags ^= ImGuiWindowFlags_NoResize & ImGuiWindowFlags_NoMove;

                        AsyncTask(ThreadPoolEnum::Game, []() {
                            SlateStyleLayout::Get()->LoadLayoutCache();
                        });
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

                        m_WindowFlags |= ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
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

            ImGuiH::CustomMaterialImage(m_ViewportContext.get(), m_PanelSize);
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

    std::pair<uint32_t, uint32_t> ImguiViewport::GetMousePosInViewport() const
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

    void ImguiViewport::Toggle() const
    {
        SPICES_PROFILE_ZONE;

        m_ToggleStateList->ResetState();
    }

    bool ImguiViewport::OnSlateResize(SlateResizeEvent& event) const
    {
        SPICES_PROFILE_ZONE;

        /**
        * @brief Recreate SceneColor SlateImage.
        */
        m_ViewportContext->ReBuildTextureID();

        /**
        * @brief Do not block the event.
        */
        return false;
    }

    bool ImguiViewport::OnWindowResizeOver(WindowResizeOverEvent& event) const
    {
        SPICES_PROFILE_ZONE;

        /**
        * @brief Recreate SceneColor SlateImage.
        */
        m_ViewportContext->ReBuildTextureID();

        /**
        * @brief Do not block the event.
        */
        return false;
    }

    bool ImguiViewport::OnToggleSlate(KeyPressedEvent& event) const
    {
		SPICES_PROFILE_ZONE;

		if (event.GetKeyCode() == Key::F11)
		{
            Toggle();
		}

		return false;
	}
}