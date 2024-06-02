/**
* @file ImguiViewportMenu.cpp.
* @brief The ImguiViewportMenu Class Implementation.
* @author Spiecs.
*/

#include "Pchheader.h"
#include "ImguiViewportMenu.h"

#include "Systems/SlateSystem.h"

namespace Spiecs {

    ImguiViewportMenu::ImguiViewportMenu(
        const std::string& panelName,
        FrameInfo& frameInfo
    )
        : ImguiSlate(panelName, frameInfo)
    {}

    void ImguiViewportMenu::OnRender()
    {
        SPIECS_PROFILE_ZONE;

        /**
        * @brief Begin render Utilities.
        */
        if (ImGui::BeginMenu(m_PanelName.c_str()))
        {
            /**
            * @brief Render Profiler.
            */
            if (ImGui::MenuItem("Profiler")) {  }

            /**
            * @brief End render Utilities.
            */
            ImGui::EndMenu();
        }
    }
}