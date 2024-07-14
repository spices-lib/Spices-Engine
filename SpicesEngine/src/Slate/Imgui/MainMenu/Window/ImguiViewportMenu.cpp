/**
* @file ImguiViewportMenu.cpp.
* @brief The ImguiViewportMenu Class Implementation.
* @author Spices.
*/

#include "Pchheader.h"
#include "ImguiViewportMenu.h"

#include "Systems/SlateSystem.h"

namespace Spices {

    ImguiViewportMenu::ImguiViewportMenu(
        const std::string& panelName ,
        FrameInfo&         frameInfo
    )
        : ImguiSlate(panelName, frameInfo)
    {}

    void ImguiViewportMenu::OnRender()
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
            if (ImGui::MenuItem(ICON_TEXT(ICON_MD_CHECK, Viewport0))) {}
            if (ImGui::MenuItem(ICON_TEXT(ICON_MD_CHECK, Viewport1))) {}

            /**
            * @brief End render Utilities.
            */
            ImGui::EndMenu();
        }
    }
}