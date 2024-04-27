#include "Pchheader.h"
#include "ImguiUtils.h"

namespace Spiecs {

    void ImguiSlate::OnRender()
    {
        ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
        if (viewportPanelSize.x != m_PanelSize.x || viewportPanelSize.y != m_PanelSize.y)
        {
            m_PanelSize = viewportPanelSize;

            WindowOnResizedEvent event(m_PanelSize.x, m_PanelSize.y);
            //OnEvent(event);
        }
    }
}