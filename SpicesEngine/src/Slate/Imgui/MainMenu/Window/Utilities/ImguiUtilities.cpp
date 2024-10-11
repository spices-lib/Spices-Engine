/**
* @file ImguiUtilities.cpp.
* @brief The ImguiUtilities Class Implementation.
* @author Spices.
*/

#include "Pchheader.h"
#include "ImguiUtilities.h"
#include "Systems/SlateSystem.h"
#include "Debugger/ImguiDebugger.h"

namespace Spices {

    ImguiUtilities::ImguiUtilities(
        const std::string& panelName ,
        FrameInfo&         frameInfo
    )
        : ImguiSlate(panelName, frameInfo)
    {
        SPICES_PROFILE_ZONE;

        /**
        * @brief Instance.
        */
        m_ImguiDebugger = SlateSystem::GetRegister()->Register<ImguiDebugger>(false, "Debugger");
    }

    void ImguiUtilities::OnRender()
    {
        SPICES_PROFILE_ZONE;

        /**
        * @brief Begin render Utilities.
        */
        std::stringstream ss;
        ss << ICON_EMPTY << " " << m_PanelName.c_str();
        if (ImGui::BeginMenu(ss.str().c_str()))
        {
            /**
            * @brief Render Profiler.
            */
            if (ImGui::MenuItem(ICON_TEXT(ICON_MD_CHECK, Actions))) {}
            if (ImGui::MenuItem(ICON_TEXT(ICON_MD_CHECK, Activity Progress))) {}
            if (ImGui::MenuItem(ICON_TEXT(ICON_MD_CHECK, Asset Validator))) {}
            if (ImGui::MenuItem(ICON_TEXT(ICON_MD_CHECK, Console))) {}
            if (ImGui::BeginMenu(ICON_TEXT(ICON_MD_CHECK, Debugger))) { m_ImguiDebugger->OnRender(); }
            if (ImGui::MenuItem(ICON_TEXT(ICON_MD_CHECK, Scene Optimizer))) {}
            if (ImGui::MenuItem(ICON_TEXT(ICON_MD_CHECK, Statistics))) {}
            if (ImGui::MenuItem(ICON_TEXT(ICON_MD_CHECK, USD Paths))) {}

            /**
            * @brief End render Utilities.
            */
            ImGui::EndMenu();
        }
    }
}