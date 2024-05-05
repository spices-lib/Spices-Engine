#include "Pchheader.h"
#include "ImguiContentBrowser.h"

namespace Spiecs {

    void ImguiContentBrowser::OnRender()
    {
        Begin();

        ImGui::ShowDemoWindow();
        ImGui::ShowDebugLogWindow();

        ImPlot::ShowDemoWindow();

        End();
    }
}