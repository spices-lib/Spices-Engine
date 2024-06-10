/**
* @file ImguiContentBrowser.cpp.
* @brief The ImguiContentBrowser Class Implementation.
* @author Spiecs.
*/

#include "Pchheader.h"
#include "ImguiContentBrowser.h"

namespace Spiecs {

    void ImguiContentBrowser::OnRender()
    {
        SPIECS_PROFILE_ZONE;

        /**
        * @brief Begin render ContentBrowser.
        */
        Begin();

        /**
        * @todo Remove it.
        */
        /*ImGui::ShowDemoWindow();
        ImGui::ShowDebugLogWindow();
        ImPlot::ShowDemoWindow();*/

        /**
        * @brief End render ContentBrowser.
        */
        End();
    }
}