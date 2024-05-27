#include "Pchheader.h"
#include "ImguiUtilities.h"
#include "Systems/SlateSystem.h"

namespace Spiecs {
    ImguiUtilities::ImguiUtilities(const std::string& panelName, FrameInfo& frameInfo)
        : ImguiSlate(panelName, frameInfo)
    {

    }

    void ImguiUtilities::OnRender()
    {
        if (ImGui::BeginMenu(m_PanelName.c_str()))
        {
            if (ImGui::MenuItem("Profiler")) {}

            ImGui::EndMenu();
        }
    }
}