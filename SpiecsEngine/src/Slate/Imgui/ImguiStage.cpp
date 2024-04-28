#include "Pchheader.h"
#include "ImguiStage.h"

namespace Spiecs {

    ImguiStage::ImguiStage(const std::string& panelName)
        : ImguiSlate(panelName)
    {}

    int bar_data[5] = {1, 2, 3, 4, 5};

    void ImguiStage::OnRender()
    {
        Begin();


        ImPlot::ShowDemoWindow();

        End();
    }
}