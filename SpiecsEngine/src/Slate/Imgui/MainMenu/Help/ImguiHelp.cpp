/**
* @file ImguiGizmos.cpp.
* @brief The ImguiGizmos Class Implementation.
* @author Spiecs.
*/

#include "Pchheader.h"
#include "ImguiHelp.h"

#include "Systems/SlateSystem.h"
#include "ImguiAbout.h"

namespace Spiecs {

    ImguiHelp::ImguiHelp(
        const std::string& panelName , 
        FrameInfo&         frameInfo
    )
        : ImguiSlate(panelName, frameInfo)
    {
        SPIECS_PROFILE_ZONE;

        /**
       * @brief Instance a ImguiAbout.
       */
        m_About = SlateSystem::GetRegister()->Register<ImguiAbout>(true, "About SpiecsEngine");
    }

    void ImguiHelp::OnRender()
    {
        SPIECS_PROFILE_ZONE;

        /**
        * @brief Begin render About.
        */
        if (ImGui::BeginMenu(m_PanelName.c_str()))
        {
            if (ImGui::MenuItem(ICON_TEXT(ICON_EMPTY, Provide Feedback))) {}
            if (ImGui::MenuItem(ICON_TEXT(ICON_EMPTY, Omni UI Docs))) {}
            if (ImGui::MenuItem(ICON_TEXT(ICON_EMPTY, User Manual), "F1")) {}
            if (ImGui::MenuItem(ICON_TEXT(ICON_EMPTY, Discover Kit SDK))) {}
            if (ImGui::MenuItem(ICON_TEXT(ICON_EMPTY, Developers Manual))) {}

            /**
            * @brief Render About.
            */
            if (ImGui::MenuItem(ICON_TEXT(ICON_EMPTY, About))) m_About->SetWindowState(true);

            /**
            * @brief End render Visualizer.
            */
            ImGui::EndMenu();
        }
    }
}