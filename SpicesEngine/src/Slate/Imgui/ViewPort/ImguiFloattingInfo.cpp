/**
* @file ImguiGizmos.cpp.
* @brief The ImguiGizmos Class Implementation.
* @author Spices.
*/

#include "Pchheader.h"
#include "ImguiFloattingInfo.h"

#include "Systems/SlateSystem.h"
#include "World/World/World.h"
#include "World/Entity.h"
#include "Core/Library/ProcessLibrary.h"

namespace Spices {

    void ImguiFloattingInfo::OnRender()
    {
        SPICES_PROFILE_ZONE;

        ImVec2 work_pos  = m_Owner->GetPanelPos();
        ImVec2 work_size = m_Owner->GetPanelSize();

        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(4.0f, 4.0f));
        ImGui::PushStyleColor(ImGuiCol_WindowBg, ImGui::GetStyleColorVec4(ImGuiCol_FrameBg));

        static int location = 1;
        ImGuiIO& io = ImGui::GetIO();
        ImGuiWindowFlags window_flags =
            ImGuiWindowFlags_NoDecoration        | 
            ImGuiWindowFlags_NoDocking           | 
            ImGuiWindowFlags_AlwaysAutoResize    | 
            ImGuiWindowFlags_NoSavedSettings     | 
            ImGuiWindowFlags_NoFocusOnAppearing  | 
            ImGuiWindowFlags_NoNav               |
            ImGuiWindowFlags_NoTitleBar;

        if (location >= 0)
        {
            const float PAD = ImGui::GetStyle().WindowPadding.x;
            
            ImVec2 window_pos, window_pos_pivot;
            window_pos.x = (location & 1) ? (work_pos.x + work_size.x - PAD) : (work_pos.x + PAD);
            window_pos.y = (location & 2) ? (work_pos.y + work_size.y - PAD) : (work_pos.y + PAD + ImGuiH::GetLineItemSize().x * 1.5f + 2.0f + ImGui::GetStyle().WindowPadding.y);
            window_pos_pivot.x = (location & 1) ? 1.0f : 0.0f;
            window_pos_pivot.y = (location & 2) ? 1.0f : 0.0f;

            ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, window_pos_pivot);

            window_flags |= ImGuiWindowFlags_NoMove;
        }
        else if (location == -2)
        {
            // Center window
            ImGui::SetNextWindowPos(work_pos + work_size * 0.5, ImGuiCond_Always, ImVec2(0.5f, 0.5f));
            window_flags |= ImGuiWindowFlags_NoMove;
        }

        ImGui::SetNextWindowBgAlpha(0.7f); // Transparent background
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

            float size = ProcessLibrary::ProcessMemoryInUsed();
            ImGui::Text("Memory in Used: %.2f GB", size);

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

        ImGui::PopStyleColor();
        ImGui::PopStyleVar();
        ImGui::End();
    }
}