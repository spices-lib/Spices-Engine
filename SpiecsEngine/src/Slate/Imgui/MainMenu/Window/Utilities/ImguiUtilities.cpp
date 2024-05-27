#include "Pchheader.h"
#include "ImguiUtilities.h"
#include "Systems/SlateSystem.h"
#include "Core/Library/ProcessLibrary.h"

namespace Spiecs {
    ImguiUtilities::ImguiUtilities(const std::string& panelName, FrameInfo& frameInfo)
        : ImguiSlate(panelName, frameInfo)
    {

    }

    void ImguiUtilities::OnRender()
    {
        if (ImGui::BeginMenu(m_PanelName.c_str()))
        {
            if (ImGui::MenuItem("Profiler")) { ProcessLibrary::OpenProcess(m_ProfileProcessName.c_str()); }

            ImGui::EndMenu();
        }
    }
}