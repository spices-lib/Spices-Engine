#include "Pchheader.h"
#include "ImguiGizmos.h"

namespace Spiecs {

    void ImguiGizmos::OnRender()
    {
        Begin();

        ImGui::ShowDemoWindow();
        ImGui::ShowDebugLogWindow();

        ImPlot::ShowDemoWindow();

        End();
    }
}