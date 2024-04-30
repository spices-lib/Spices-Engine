#include "Pchheader.h"
#include "ImguiStage.h"

namespace Spiecs {

    ImguiStage::ImguiStage(const std::string& panelName)
        : ImguiSlate(panelName)
    {}

    void ImguiStage::OnRender()
    {
        Begin();
        End();
    }
}