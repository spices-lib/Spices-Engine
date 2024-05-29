/**
* @file ImguiGizmos.cpp.
* @brief The ImguiGizmos Class Implementation.
* @author Spiecs.
*/

#include "Pchheader.h"
#include "ImguiHelp.h"

#include "Systems/SlateSystem.h"
#include "ImguiAbout.h"

namespace Spiecs {

    ImguiHelp::ImguiHelp(
        const std::string& panelName , 
        FrameInfo&         frameInfo
    )
        : ImguiSlate(panelName, frameInfo)
    {
        ZoneScoped;

        /**
       * @brief Instance a ImguiAbout.
       */
        m_About = SlateSystem::GetRegister()->Register<ImguiAbout>(true, "About SpiecsEngine");
    }

    void ImguiHelp::OnRender()
    {
        ZoneScoped;

        /**
        * @brief Begin render About.
        */
        if (ImGui::BeginMenu(m_PanelName.c_str()))
        {
            /**
            * @brief Render About.
            */
            if (ImGui::MenuItem("About")) m_About->SetWindowState(true);

            /**
            * @brief End render Visualizer.
            */
            ImGui::EndMenu();
        }
    }
}