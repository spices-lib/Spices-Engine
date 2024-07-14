/**
* @file ImguiOperation.cpp.
* @brief The ImguiOperation Class Implementation.
* @author Spices.
*/

#include "Pchheader.h"
#include "ImguiInfoBar.h"

namespace Spices {

    void ImguiInfoBar::OnRender()
    {
        SPICES_PROFILE_ZONE;
        
        /**
        * @brief Begin render ContentBrowser.
        */
        Begin(m_PanelName, 0.5f, ImGuiWindowFlags_NoTitleBar);
        
        /**
        * @brief End render ContentBrowser.
        */
        End();
    }
}