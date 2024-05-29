/**
* @file ImguiGizmos.cpp.
* @brief The ImguiGizmos Class Implementation.
* @author Spiecs.
*/

#include "Pchheader.h"
#include "ImguiVisualizer.h"

#include "Systems/SlateSystem.h"
#include "ImguiGBufferVisualizer.h"

namespace Spiecs {

    ImguiVisualizer::ImguiVisualizer(
        const std::string& panelName , 
        FrameInfo&         frameInfo
    )
        : ImguiSlate(panelName, frameInfo)
    {
        ZoneScoped;

        /**
        * @brief Instance a ImguiGBufferVisualizer.
        */
        m_GBufferVisualizer = SlateSystem::GetRegister()->Register<ImguiGBufferVisualizer>(true, "GBuffer");
    }

    void ImguiVisualizer::OnRender()
    {
        ZoneScoped;

        /**
        * @brief Begin render Visualizer.
        */
        if (ImGui::BeginMenu(m_PanelName.c_str()))
        {
            /**
            * @brief Render GBufferVisualizer.
            */
            if (ImGui::MenuItem("GBuffer")) m_GBufferVisualizer->SetWindowState(true);

            /**
            * @brief End render Visualizer.
            */
            ImGui::EndMenu();
        }
    }
}