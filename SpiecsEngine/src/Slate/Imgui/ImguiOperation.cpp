/**
* @file ImguiOperation.cpp.
* @brief The ImguiOperation Class Implementation.
* @author Spiecs.
*/

#include "Pchheader.h"
#include "ImguiOperation.h"

namespace Spiecs {

    void ImguiOperation::OnRender()
    {
        SPIECS_PROFILE_ZONE;
        
        /**
        * @brief Begin render ContentBrowser.
        */
        Begin(m_PanelName, 0.5f, ImGuiWindowFlags_NoTitleBar);

        const ImVec2 buttonSize = ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().x);
        const float fontSize = ImGui::GetContentRegionAvail().x / ImGuiH::GetLineItemSize().x;

        {
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


        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(2.0f, 4.0f));
        ImGui::SetWindowFontScale(fontSize);
        ImGui::Button(ICON_MD_PLAY_ARROW, buttonSize);
        ImGui::Button(ICON_MD_PAUSE, buttonSize);
        ImGui::Button(ICON_MD_STOP, buttonSize);
        ImGui::Button(ICON_MD_FORMAT_PAINT, buttonSize);
        ImGui::Button(ICON_MD_CROP_ROTATE, buttonSize);
        ImGui::Button(ICON_MD_LINEAR_SCALE, buttonSize);
        ImGui::Button(ICON_MD_MOUSE, buttonSize);
        ImGui::Button(ICON_MD_SNAPCHAT, buttonSize);

        ImGui::SetWindowFontScale(1.0f);
        ImGui::PopStyleVar();
        /**
        * @brief End render ContentBrowser.
        */
        End();
    }
}