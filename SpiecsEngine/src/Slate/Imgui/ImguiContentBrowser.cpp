#include "Pchheader.h"
#include "ImguiContentBrowser.h"

namespace Spiecs {

    void ImguiContentBrowser::OnRender()
    {
        Begin();
        ImPlot::ShowDemoWindow();
        End();
    }
}