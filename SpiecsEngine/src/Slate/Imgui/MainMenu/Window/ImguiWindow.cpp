/**
* @file ImguiGizmos.cpp.
* @brief The ImguiGizmos Class Implementation.
* @author Spiecs.
*/

#include "Pchheader.h"
#include "Systems/SlateSystem.h"

#include "ImguiWindow.h"
#include "Visualizer/ImguiVisualizer.h"
#include "Utilities/ImguiUtilities.h"
#include "ImguiViewportMenu.h"

namespace Spiecs {

    ImguiWindow::ImguiWindow(
        const std::string& panelName , 
        FrameInfo&         frameInfo
    )
        : ImguiSlate(panelName, frameInfo)
    {
        SPIECS_PROFILE_ZONE;

        /**
        * @brief Instance a Visualizer.
        */
        m_Visualizer = SlateSystem::GetRegister()->Register<ImguiVisualizer>(false, "Visualizer");

        /**
        * @brief Instance a Utilities.
        */
        m_Utilities = SlateSystem::GetRegister()->Register<ImguiUtilities>(false, "Utilities");

        /**
        * @brief Instance a ViewportMenu.
        */
        m_ViewportMenu = SlateSystem::GetRegister()->Register<ImguiViewportMenu>(false, "ViewportMenu");
    }

    void ImguiWindow::OnRender()
    {
        SPIECS_PROFILE_ZONE;

        /**
        * @brief Begin render Window.
        */
        if (ImGui::BeginMenu(m_PanelName.c_str()))
        {




            /**
            * @brief Render Visualizer.
            */
            m_Visualizer->OnRender();

            /**
            * @brief Render Utilities.
            */
            m_Utilities->OnRender();

            /**
            * @brief Render Utilities.
            */
            m_ViewportMenu->OnRender();

            /**
            * @brief End Windows.
            */
            ImGui::EndMenu();
        }
    }
}