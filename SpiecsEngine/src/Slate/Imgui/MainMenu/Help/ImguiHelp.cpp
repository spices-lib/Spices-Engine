#include "Pchheader.h"
#include "Systems/SlateSystem.h"
#include "ImguiHelp.h"

#include "ImguiAbout.h"

namespace Spiecs {

    ImguiHelp::ImguiHelp(const std::string& panelName, FrameInfo& frameInfo)
        : ImguiSlate(panelName, frameInfo)
    {
        ZoneScoped;

        m_About = SlateSystem::GetRegister()->Register<ImguiAbout>(true, "About SpiecsEngine");
    }

    void ImguiHelp::OnRender()
    {
        ZoneScoped;

        if (ImGui::BeginMenu(m_PanelName.c_str()))
        {
            if (ImGui::MenuItem("About")) m_About->SetWindowState(true);

            ImGui::EndMenu();
        }
    }
}