/**
* @file ImguiPreferences.cpp.
* @brief The ImguiPreferences Class Implementation.
* @author Spices.
*/

#include "Pchheader.h"
#include "ImguiPreferences.h"

namespace Spices {

    ImguiPreferences::ImguiPreferences(
        const std::string& panelName ,
        FrameInfo&         frameInfo
    )
        : ImguiSlate(panelName, frameInfo)
    {
        SPICES_PROFILE_ZONE;
    }

    void ImguiPreferences::OnRender()
    {
        SPICES_PROFILE_ZONE;


    }
}