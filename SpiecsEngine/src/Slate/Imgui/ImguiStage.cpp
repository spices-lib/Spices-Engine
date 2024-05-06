#include "Pchheader.h"
#include "ImguiStage.h"

namespace Spiecs {

    ImguiStage::ImguiStage(const std::string& panelName, FrameInfo& frameInfo)
        : ImguiSlate(panelName, frameInfo)
    {}

    void ImguiStage::OnRender()
    {
        Begin();
        End();
    }
}