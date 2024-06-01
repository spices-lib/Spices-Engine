/**
* @file ImguiViewportMenu.cpp.
* @brief The ImguiViewportMenu Class Implementation.
* @author Spiecs.
*/

#include "Pchheader.h"
#include "ImguiViewportMenu.h"

#include "Systems/SlateSystem.h"

namespace Spiecs {

    ImguiViewportMenu::ImguiViewportMenu(
        const std::string& panelName,
        FrameInfo& frameInfo
    )
        : ImguiSlate(panelName, frameInfo)
    {}

    void ImguiViewportMenu::OnRender()
    {
        ZoneScoped;

        /**
        * @brief Begin render ImguiViewportMenu.
        */
        if (ImGui::BeginMenu("Viewport"))
        {
            for (int i = 0; i < MaxViewportNum; i++)
            {
                ImGui::Checkbox("##", &m_ViewportVisible[i]);
                ImGui::SameLine();

                std::stringstream ss;
                ss << "Viewport" << i;

                if (ImGui::MenuItem(ss.str().c_str()))
                {
                    m_ViewportVisible[i] = !m_ViewportVisible[i];
                }
            }

            /**
            * @brief End Windows.
            */
            ImGui::EndMenu();
        }
    }
}