#include "Pchheader.h"
#include "Systems/SlateSystem.h"
#include "ImguiWindow.h"
#include "Visualizer/ImguiVisualizer.h"

namespace Spiecs {

    ImguiWindow::ImguiWindow(const std::string& panelName, FrameInfo& frameInfo)
        : ImguiSlate(panelName, frameInfo)
    {
        m_Visualizer = SlateSystem::GetRegister()->Register<ImguiVisualizer>(false, "Visualizer");
    }

    void ImguiWindow::OnRender()
    {
        if (ImGui::BeginMenu(m_PanelName.c_str()))
        {
            m_Visualizer->OnRender();

            ImGui::EndMenu();
        }
    }
}