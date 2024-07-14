/**
* @file ImguiGizmos.cpp.
* @brief The ImguiGizmos Class Implementation.
* @author Spices.
*/

#include "Pchheader.h"
#include "ImguiVisualizer.h"

#include "Systems/SlateSystem.h"
#include "ImguiGBufferVisualizer.h"

namespace Spices {

    ImguiVisualizer::ImguiVisualizer(
        const std::string& panelName , 
        FrameInfo&         frameInfo
    )
        : ImguiSlate(panelName, frameInfo)
    {
        SPICES_PROFILE_ZONE;

        /**
        * @brief Instance a ImguiGBufferVisualizer.
        */
        m_GBufferVisualizer = SlateSystem::GetRegister()->Register<ImguiGBufferVisualizer>(true, "GBuffer");
    }

    void ImguiVisualizer::OnRender()
    {
        SPICES_PROFILE_ZONE;

        /**
        * @brief Begin render Visualizer.
        */
        std::stringstream ss;
        ss << ICON_EMPTY << " " << m_PanelName.c_str();
        if (ImGui::BeginMenu(ss.str().c_str()))
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