/**
* @file ImguiPreferences.cpp.
* @brief The ImguiPreferences Class Implementation.
* @author Spices.
*/

#include "Pchheader.h"
#include "ImguiPreferences.h"

#include "Systems/SlateSystem.h"
#include "ImguiDisplayOptions.h"

namespace Spices {

    ImguiPreferences::ImguiPreferences(
        const std::string& panelName ,
        FrameInfo&         frameInfo
    )
        : ImguiSlate(panelName, frameInfo)
    {
        SPICES_PROFILE_ZONE;

        /**
        * @brief Instance.
        */
        m_ImguiDisplayOptions = SlateSystem::GetRegister()->Register<ImguiDisplayOptions>(false, "DisplayOptions");
    }

    void ImguiPreferences::OnRender()
    {
        SPICES_PROFILE_ZONE;
        
        if (!m_IsSlateOn) return;

        /**
        * @brief Begin render Preferences.
        */
        Begin();

        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(2.0f, 4.0f));

        {
            SPICES_PROFILE_ZONEN("ImguiPreferences::Reset");

            ImGui::Spacing();
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.0f, 0.0f, 1.0f, 1.0f));
            ImGui::Button("Reset to Default");
            ImGui::PopStyleColor();
            ImGui::Spacing();
        }

        const float columeWidth = ImGuiH::GetLineItemSize().x * 9.0f;
        static ImguiSlate* selectedSlate = nullptr;

        {
            SPICES_PROFILE_ZONEN("ImguiPreferences::ItemLists");

            ImGui::PushID("ImguiPreferences::ItemLists");
            ImGui::Columns(2, 0, false);

            ImGui::SetColumnWidth(0, columeWidth);
           
            float itemHeight = ImGui::GetStyle().ItemSpacing.y;
            if (ImGui::BeginChild("ImguiPreferences::ItemLists", ImVec2(0, -itemHeight), ImGuiChildFlags_None, ImGuiWindowFlags_HorizontalScrollbar))
            {
                ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(6.0f, 6.0f));
                ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(4.0f, 4.0f));

                if (ImGui::MenuItem(ICON_TEXT(" ", Animation))) {}
                if (ImGui::MenuItem(ICON_TEXT(" ", Audio))) {}
                if (ImGui::MenuItem(ICON_TEXT(" ", Capture Screenshot))) {}
                if (ImGui::MenuItem(ICON_TEXT(" ", Curve Tools))) {}
                if (ImGui::MenuItem(ICON_TEXT(" ", Datetime Format))) {}
                if (ImGui::MenuItem(ICON_TEXT(" ", Display Options))) { selectedSlate = m_ImguiDisplayOptions.get(); }
                if (ImGui::MenuItem(ICON_TEXT(" ", Environment))) {}
                if (ImGui::MenuItem(ICON_TEXT(" ", Live))) {}
                if (ImGui::MenuItem(ICON_TEXT(" ", Material))) {}
                if (ImGui::MenuItem(ICON_TEXT(" ", Material Thumbnail))) {}
                if (ImGui::MenuItem(ICON_TEXT(" ", Measure))) {}
                if (ImGui::MenuItem(ICON_TEXT(" ", Metrics Assembler))) {}
                if (ImGui::MenuItem(ICON_TEXT(" ", Navigation))) {}
                if (ImGui::MenuItem(ICON_TEXT(" ", Particle System))) {}
                if (ImGui::MenuItem(ICON_TEXT(" ", Physics))) {}
                if (ImGui::MenuItem(ICON_TEXT(" ", Property Widgets))) {}
                if (ImGui::MenuItem(ICON_TEXT(" ", Rendering))) {}
                if (ImGui::MenuItem(ICON_TEXT(" ", Resource Monitor))) {}
                if (ImGui::MenuItem(ICON_TEXT(" ", Stage))) {}
                if (ImGui::MenuItem(ICON_TEXT(" ", Tagging))) {}
                if (ImGui::MenuItem(ICON_TEXT(" ", Template Startup))) {}
                if (ImGui::MenuItem(ICON_TEXT(" ", Thumbnail Generation))) {}
                if (ImGui::MenuItem(ICON_TEXT(" ", Viewport))) {}
                if (ImGui::MenuItem(ICON_TEXT(" ", Visual Scripting))) {}
                if (ImGui::MenuItem(ICON_TEXT(" ", Visualization))) {}

                ImGui::PopStyleVar(2);
            }
            ImGui::EndChild();

            ImGui::NextColumn();
        }

        {
            SPICES_PROFILE_ZONEN("ImguiPreferences::Item");

			float itemHeight = ImGui::GetStyle().ItemSpacing.y;
			if (ImGui::BeginChild("ImguiPreferences::Item", ImVec2(0, -itemHeight), ImGuiChildFlags_None, ImGuiWindowFlags_HorizontalScrollbar))
			{
				ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4, 1));

                if (selectedSlate)
                {
                    selectedSlate->OnRender();
                }

				ImGui::PopStyleVar();
			}
			ImGui::EndChild();

            ImGui::Columns(1);
            ImGui::PopID();
        }

        ImGui::PopStyleVar();

        /**
        * @brief End render GBuffer Preferences.
        */
        End();
    }
}