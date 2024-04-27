#include "Pchheader.h"
#include "ImguiContentBrowser.h"

namespace Spiecs {

    void ImguiContentBrowser::OnRender()
    {
        ImGui::Begin(m_PanelName.c_str());

        ImGui::End();
    }
}