/**
* @file ImguiOperation.cpp.
* @brief The ImguiOperation Class Implementation.
* @author Spiecs.
*/

#include "Pchheader.h"
#include "ImguiInfoBar.h"

namespace Spiecs {

    void ImguiInfoBar::OnRender()
    {
        SPIECS_PROFILE_ZONE;
        
        /**
        * @brief Begin render ContentBrowser.
        */
        Begin(m_PanelName, 0.5f, ImGuiWindowFlags_NoTitleBar);

        ImGui::Separator();
        
        /**
        * @brief End render ContentBrowser.
        */
        End();
    }
}