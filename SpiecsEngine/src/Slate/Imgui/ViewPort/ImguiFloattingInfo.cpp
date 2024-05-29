/**
* @file ImguiGizmos.cpp.
* @brief The ImguiGizmos Class Implementation.
* @author Spiecs.
*/

#include "Pchheader.h"
#include "ImguiFloattingInfo.h"

#include "Systems/SlateSystem.h"
#include "World/World/World.h"
#include "World/Entity.h"

namespace Spiecs {

    void ImguiFloattingInfo::OnRender()
    {
        ZoneScoped;

        const ImGuiViewport* viewport = ImGui::GetMainViewport();

        ImVec2 work_pos  = m_Owner->GetPanelPos();
        ImVec2 work_size = m_Owner->GetPanelSize();

        static int location = 1;
        ImGuiIO& io = ImGui::GetIO();
        ImGuiWindowFlags window_flags = 
            ImGuiWindowFlags_NoDecoration        | 
            ImGuiWindowFlags_NoDocking           | 
            ImGuiWindowFlags_AlwaysAutoResize    | 
            ImGuiWindowFlags_NoSavedSettings     | 
            ImGuiWindowFlags_NoFocusOnAppearing  | 
            ImGuiWindowFlags_NoNav;

        if (location >= 0)
        {
            const float PAD = 10.0f;
            
            ImVec2 window_pos, window_pos_pivot;
            window_pos.x = (location & 1) ? (work_pos.x + work_size.x - PAD) : (work_pos.x + PAD);
            window_pos.y = (location & 2) ? (work_pos.y + work_size.y - PAD) : (work_pos.y + PAD);
            window_pos_pivot.x = (location & 1) ? 1.0f : 0.0f;
            window_pos_pivot.y = (location & 2) ? 1.0f : 0.0f;

            ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, window_pos_pivot);
            ImGui::SetNextWindowViewport(viewport->ID);

            window_flags |= ImGuiWindowFlags_NoMove;
        }
        else if (location == -2)
        {
            // Center window
            ImGui::SetNextWindowPos(work_pos + work_size * 0.5, ImGuiCond_Always, ImVec2(0.5f, 0.5f));
            window_flags |= ImGuiWindowFlags_NoMove;
        }

        ImGui::SetNextWindowBgAlpha(0.35f); // Transparent background
        bool open = true;
        if (ImGui::Begin(m_PanelName.c_str(), &open, window_flags))
        {
            ImGui::Text("FPS: %.2f | Frame time: %.2f ms", io.Framerate, 1000.0f / io.Framerate);
            ImGui::Text("ViewPort Size: ( %d, %d )", (int)work_size.x, (int)work_size.y);

            if (ImGui::IsMousePosValid())
            {
                ImGui::Text("Mouse Position: ( %d, %d )", (int)io.MousePos.x, (int)io.MousePos.y);
            }
            else
            {
                ImGui::Text("Mouse Position: <invalid>");
            }

            if (ImGui::BeginPopupContextWindow())
            {
                if (ImGui::MenuItem("Custom",       NULL, location == -1)) location = -1;
                if (ImGui::MenuItem("Center",       NULL, location == -2)) location = -2;
                if (ImGui::MenuItem("Top-left",     NULL, location == 0))  location = 0;
                if (ImGui::MenuItem("Top-right",    NULL, location == 1))  location = 1;
                if (ImGui::MenuItem("Bottom-left",  NULL, location == 2))  location = 2;
                if (ImGui::MenuItem("Bottom-right", NULL, location == 3))  location = 3;
                if (open && ImGui::MenuItem("Close")) open = false;
                ImGui::EndPopup();
            }
        }

        ImGui::End();
    }
}