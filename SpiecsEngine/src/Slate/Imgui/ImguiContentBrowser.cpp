#include "Pchheader.h"
#include "ImguiContentBrowser.h"

namespace Spiecs {

    void ImguiContentBrowser::OnRender()
    {
        ZoneScoped;

        Begin();

        ImGui::ShowDemoWindow();
        ImGui::ShowDebugLogWindow();

        ImPlot::ShowDemoWindow();

        End();
    }
}