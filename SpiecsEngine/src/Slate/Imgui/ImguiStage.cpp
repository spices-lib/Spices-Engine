#include "Pchheader.h"
#include "ImguiStage.h"

namespace Spiecs {

    void ImguiStage::OnRender()
    {
        ImGui::Begin(m_PanelName.c_str());

        ImGui::End();
    }
}