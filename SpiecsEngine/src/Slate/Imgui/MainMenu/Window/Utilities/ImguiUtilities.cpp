/**
* @file ImguiGizmos.cpp.
* @brief The ImguiGizmos Class Implementation.
* @author Spiecs.
*/

#include "Pchheader.h"
#include "ImguiUtilities.h"

#include "Systems/SlateSystem.h"
#include "Core/Library/ProcessLibrary.h"

namespace Spiecs {

    ImguiUtilities::ImguiUtilities(
        const std::string& panelName ,
        FrameInfo&         frameInfo
    )
        : ImguiSlate(panelName, frameInfo)
    {}

    void ImguiUtilities::OnRender()
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
            if (ImGui::MenuItem("Profiler")) { ProcessLibrary::OpenProcess(m_ProfileProcessName.c_str()); }

            /**
            * @brief End render Utilities.
            */
            ImGui::EndMenu();
        }
    }
}