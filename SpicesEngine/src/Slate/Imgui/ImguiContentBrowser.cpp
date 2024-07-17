/**
* @file ImguiContentBrowser.cpp.
* @brief The ImguiContentBrowser Class Implementation.
* @author Spices.
*/

#include "Pchheader.h"
#include "ImguiContentBrowser.h"

namespace Spices {

    void ImguiContentBrowser::OnRender()
    {
        SPICES_PROFILE_ZONE;

        /**
        * @brief Begin render ContentBrowser.
        */
        Begin();

        /**
        * @todo Remove it.
        */
        ImGui::ShowDemoWindow();
        //ImGui::ShowDebugLogWindow();
        //ImPlot::ShowDemoWindow();

        /**
        * @brief End render ContentBrowser.
        */
        End();
    }
}