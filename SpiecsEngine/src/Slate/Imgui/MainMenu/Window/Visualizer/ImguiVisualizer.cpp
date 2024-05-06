#include "Pchheader.h"
#include "ImguiVisualizer.h"
#include "Systems/SlateSystem.h"
#include "ImguiGBufferVisualizer.h"

namespace Spiecs {
    ImguiVisualizer::ImguiVisualizer(const std::string& panelName, FrameInfo& frameInfo)
        : ImguiSlate(panelName, frameInfo)
    {
        m_GBufferVisualizer = SlateSystem::GetRegister()->Register<ImguiGBufferVisualizer>(true, "GBuffer");
    }

    void ImguiVisualizer::OnRender()
    {
        if (ImGui::BeginMenu(m_PanelName.c_str()))
        {
            if (ImGui::MenuItem("GBuffer")) m_GBufferVisualizer->SetWindowState(true);

            ImGui::EndMenu();
        }
    }
}