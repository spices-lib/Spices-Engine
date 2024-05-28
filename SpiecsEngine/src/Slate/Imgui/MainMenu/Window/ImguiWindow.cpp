#include "Pchheader.h"
#include "Systems/SlateSystem.h"
#include "ImguiWindow.h"
#include "Visualizer/ImguiVisualizer.h"
#include "Utilities/ImguiUtilities.h"

namespace Spiecs {

    ImguiWindow::ImguiWindow(const std::string& panelName, FrameInfo& frameInfo)
        : ImguiSlate(panelName, frameInfo)
    {
        ZoneScoped;

        m_Visualizer = SlateSystem::GetRegister()->Register<ImguiVisualizer>(false, "Visualizer");
        m_Utilities  = SlateSystem::GetRegister()->Register<ImguiUtilities>(false, "Utilities");
    }

    void ImguiWindow::OnRender()
    {
        ZoneScoped;

        if (ImGui::BeginMenu(m_PanelName.c_str()))
        {
            m_Visualizer->OnRender();
            m_Utilities->OnRender();

            ImGui::EndMenu();
        }
    }
}