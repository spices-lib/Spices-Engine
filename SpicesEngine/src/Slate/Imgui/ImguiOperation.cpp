/**
* @file ImguiOperation.cpp.
* @brief The ImguiOperation Class Implementation.
* @author Spices.
*/

#include "Pchheader.h"
#include "ImguiOperation.h"

namespace Spices {

    void ImguiOperation::OnRender()
    {
        SPICES_PROFILE_ZONE;
        
        /**
        * @brief Begin render ContentBrowser.
        */
        Begin(m_PanelName, 0.5f, ImGuiWindowFlags_NoTitleBar);

        const ImVec2 buttonSize = ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().x);
        const float fontSize = ImGui::GetContentRegionAvail().x / ImGuiH::GetLineItemSize().x;

        {
            SPICES_PROFILE_ZONEN("ImguiOperation Separator");

            ImGui::Spacing();
            ImGui::Spacing();
            ImGui::Spacing();
            ImGui::SetWindowFontScale(0.2f);
            ImGui::PushStyleColor(ImGuiCol_Separator, ImVec4(0.19f, 0.19f, 0.19f, 1.0f));
            ImGui::PushStyleVar(ImGuiStyleVar_SeparatorTextBorderSize, 2.0f);
            ImGui::SeparatorText("##");
            ImGui::SeparatorText("##");
            ImGui::SeparatorText("##");
            ImGui::SetWindowFontScale(1.0f);
            ImGui::PopStyleVar();
            ImGui::PopStyleColor();
            ImGui::Spacing();
            ImGui::Spacing();
            ImGui::Spacing();
        }

        {
            SPICES_PROFILE_ZONEN("ImguiOperation Icon");

            ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(2.0f, 4.0f));
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.567f, 0.629f, 0.641f, 1.0f));
            ImGui::SetWindowFontScale(fontSize);


            ImGui::Button(ICON_MD_VIEW_IN_AR, buttonSize);
            ImGui::SetItemTooltip("All Model Kinds(T)");
            ImGui::Button(ICON_EMPTY, buttonSize);
            ImGui::SetItemTooltip("Select(Q)");
            ImGui::Button(ICON_MD_OPEN_WITH, buttonSize);
            ImGui::SetItemTooltip("Move(W)");
            ImGui::Button(ICON_MD_FLIP_CAMERA_ANDROID, buttonSize);
            ImGui::SetItemTooltip("Rotate(E)");
            ImGui::Button(ICON_MD_ZOOM_OUT_MAP, buttonSize);
            ImGui::SetItemTooltip("Scale(R)");
            ImGui::Separator();
            ImGui::Button(ICON_EMPTY, buttonSize);
            ImGui::SetItemTooltip("Snap(S)");
            ImGui::Separator();
            ImGui::Button(ICON_MD_PLAY_ARROW, buttonSize);
            ImGui::SetItemTooltip("Play(Space)");
            ImGui::Button(ICON_MD_PAUSE, buttonSize);
            ImGui::SetItemTooltip("Pause(Space)");
            ImGui::Button(ICON_MD_STOP, buttonSize);
            ImGui::SetItemTooltip("Stop");
            ImGui::Separator();
            ImGui::Button(ICON_MD_BRUSH, buttonSize);
            ImGui::SetItemTooltip("Paint CTRL + B");
            ImGui::Button(ICON_EMPTY, buttonSize);
            ImGui::SetItemTooltip("Zero Gravity(X)");

            ImGui::SetWindowFontScale(1.0f);
            ImGui::PopStyleColor();
            ImGui::PopStyleVar();
        }
        /**
        * @brief End render ContentBrowser.
        */
        End();
    }
}