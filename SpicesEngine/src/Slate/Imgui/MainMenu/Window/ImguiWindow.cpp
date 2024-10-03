/**
* @file ImguiWindow.cpp.
* @brief The ImguiWindow Class Implementation.
* @author Spices.
*/

#include "Pchheader.h"
#include "Systems/SlateSystem.h"

#include "ImguiWindow.h"
#include "Visualizer/ImguiVisualizer.h"
#include "Utilities/ImguiUtilities.h"
#include "ImguiViewportMenu.h"

namespace Spices {

    ImguiWindow::ImguiWindow(
        const std::string& panelName , 
        FrameInfo&         frameInfo
    )
        : ImguiSlate(panelName, frameInfo)
    {
        SPICES_PROFILE_ZONE;

        /**
        * @brief Instance a Visualizer.
        */
        m_Visualizer = SlateSystem::GetRegister()->Register<ImguiVisualizer>(false, "Visualizer");

        /**
        * @brief Instance a Utilities.
        */
        m_Utilities = SlateSystem::GetRegister()->Register<ImguiUtilities>(false, "Utilities");

        /**
        * @brief Instance a ViewportMenu.
        */
        m_ViewportMenu = SlateSystem::GetRegister()->Register<ImguiViewportMenu>(false, "ViewportMenu");
    }

    void ImguiWindow::OnRender()
    {
        SPICES_PROFILE_ZONE;

        if (ImGui::BeginMenu(m_PanelName.c_str()))
        {
            ImGuiH::MainMenuTitleSeparator();

            if (ImGui::BeginMenu(ICON_TEXT(ICON_EMPTY, Animation)))
            {
                if (ImGui::MenuItem(ICON_TEXT(ICON_EMPTY, Animation Graph))) {}
                if (ImGui::MenuItem(ICON_TEXT(ICON_EMPTY, Curve Editor))) {}
                if (ImGui::MenuItem(ICON_TEXT(ICON_EMPTY, Retargeting))) {}
                if (ImGui::MenuItem(ICON_TEXT(ICON_EMPTY, Sequencer))) {}
                if (ImGui::MenuItem(ICON_TEXT(ICON_EMPTY, Stage Recorder))) {}
                if (ImGui::MenuItem(ICON_TEXT(ICON_EMPTY, Timeline))) {}

                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu(ICON_TEXT(ICON_EMPTY, Browsers)))
            {
                if (ImGui::MenuItem(ICON_TEXT(ICON_MD_CHECK, Asset Stores))) {}
                if (ImGui::MenuItem(ICON_TEXT(ICON_MD_CHECK, Assets))) {}
                if (ImGui::MenuItem(ICON_TEXT(ICON_MD_CHECK, Content))) {}
                if (ImGui::MenuItem(ICON_TEXT(ICON_MD_CHECK, Environment))) {}
                if (ImGui::MenuItem(ICON_TEXT(ICON_MD_CHECK, Examples))) {}
                if (ImGui::MenuItem(ICON_TEXT(ICON_MD_CHECK, Materials))) {}
                if (ImGui::MenuItem(ICON_TEXT(ICON_MD_CHECK, Showcases))) {}
                if (ImGui::MenuItem(ICON_TEXT(ICON_MD_CHECK, SimReady Explorer))) {}

                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu(ICON_TEXT(ICON_EMPTY, Navigation)))
            {
                if (ImGui::MenuItem(ICON_TEXT(ICON_MD_CHECK, NavMesh))) {}

                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu(ICON_TEXT(ICON_EMPTY, Particles)))
            {
                if (ImGui::MenuItem(ICON_TEXT(ICON_MD_CHECK, Editor))) {}
                if (ImGui::MenuItem(ICON_TEXT(ICON_MD_CHECK, Playback Controls))) {}

                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu(ICON_TEXT(ICON_EMPTY, Renderering)))
            {
                if (ImGui::MenuItem(ICON_TEXT(ICON_MD_CHECK, AR))) {}
                if (ImGui::MenuItem(ICON_TEXT(ICON_MD_CHECK, MDL Material Graph))) {}
                if (ImGui::MenuItem(ICON_TEXT(ICON_MD_CHECK, Movie Capture))) {}
                if (ImGui::MenuItem(ICON_TEXT(ICON_MD_CHECK, Render Settings))) {}
                if (ImGui::MenuItem(ICON_TEXT(ICON_MD_CHECK, Tablet AR))) {}
                if (ImGui::MenuItem(ICON_TEXT(ICON_MD_CHECK, VR))) {}

                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu(ICON_TEXT(ICON_EMPTY, Simulation)))
            {
                ImGui::Separator();
                if (ImGui::MenuItem(ICON_TEXT(ICON_EMPTY, Monitor))) {}
                if (ImGui::MenuItem(ICON_TEXT(ICON_EMPTY, Presets))) {}
                ImGui::Separator();
                if (ImGui::MenuItem(ICON_TEXT(ICON_EMPTY, Character Controller))) {}
                if (ImGui::MenuItem(ICON_TEXT(ICON_EMPTY, Collision Groups Filtering Matrix))) {}
                if (ImGui::MenuItem(ICON_TEXT(ICON_EMPTY, Debug))) {}
                if (ImGui::MenuItem(ICON_TEXT(ICON_EMPTY, Demo Scenes))) {}
                if (ImGui::MenuItem(ICON_TEXT(ICON_EMPTY, Physics Authoring Toolbar))) {}
                if (ImGui::MenuItem(ICON_TEXT(ICON_EMPTY, Settings))) {}

                ImGui::EndMenu();
            }

            /**
            * @brief Render Visualizer.
            */
            m_Visualizer->OnRender();

            /**
            * @brief Render Utilities.
            */
            m_Utilities->OnRender();

            /**
            * @brief Render Utilities.
            */
            m_ViewportMenu->OnRender();

            if (ImGui::BeginMenu(ICON_TEXT(ICON_EMPTY, Visual Scripting)))
            {
                ImGui::Separator();
                if (ImGui::MenuItem(ICON_TEXT(ICON_EMPTY, Action Graph))) {}
                if (ImGui::MenuItem(ICON_TEXT(ICON_EMPTY, Generic Graph))) {}
                if (ImGui::MenuItem(ICON_TEXT(ICON_EMPTY, Node Description Editor))) {}
                if (ImGui::MenuItem(ICON_TEXT(ICON_EMPTY, Toolkit))) {}

                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu(ICON_TEXT(ICON_EMPTY, Warp)))
            {
                ImGui::Separator();
                if (ImGui::MenuItem(ICON_TEXT(ICON_EMPTY, Documentation))) {}
                if (ImGui::MenuItem(ICON_TEXT(ICON_EMPTY, Getting Started))) {}
                if (ImGui::MenuItem(ICON_TEXT(ICON_EMPTY, Sample Scenes))) {}

                ImGui::EndMenu();
            }

            if (ImGui::MenuItem(ICON_TEXT(ICON_MD_CHECK, Collection))) {}
            if (ImGui::MenuItem(ICON_TEXT(ICON_MD_CHECK, DeepSearch))) {}
            if (ImGui::MenuItem(ICON_TEXT(ICON_MD_CHECK, Hotkeys))) {}
            if (ImGui::MenuItem(ICON_TEXT(ICON_MD_CHECK, Layer))) {}
            if (ImGui::MenuItem(ICON_TEXT(ICON_MD_CHECK, Property))) {}
            if (ImGui::MenuItem(ICON_TEXT(ICON_MD_CHECK, Script Editor))) {}
            if (ImGui::MenuItem(ICON_TEXT(ICON_MD_CHECK, Stage))) {}
            if (ImGui::MenuItem(ICON_TEXT(ICON_MD_CHECK, ToolBar))) {}
            ImGui::Separator();
            if (ImGui::MenuItem(ICON_TEXT(ICON_MD_CHECK, Extensions))) {}

            /**
            * @brief End Windows.
            */
            ImGui::EndMenu();
        }
    }
}