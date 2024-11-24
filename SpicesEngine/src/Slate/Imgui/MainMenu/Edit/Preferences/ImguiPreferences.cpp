/**
* @file ImguiPreferences.cpp.
* @brief The ImguiPreferences Class Implementation.
* @author Spices.
*/

#include "Pchheader.h"
#include "ImguiPreferences.h"

#include "Systems/SlateSystem.h"
#include "ImguiDisplayOptions.h"
#include "ImguiPostProcessing.h"

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
        m_ImguiPostProcessing = SlateSystem::GetRegister()->Register<ImguiPostProcessing>(false, "PostProcessing");
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
            ImGui::SetWindowFontScale(1.15f);
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.204f, 0.78f, 1.0f, 1.0f));
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImGui::GetStyle().Colors[ImGuiCol_ButtonHovered]);
            ImGui::Button(" Reset to Default ");
            ImGui::PopStyleColor(2);
            ImGui::SetWindowFontScale(1.0f);
            ImGui::SetItemTooltip("This will reset all settings back to installed state after the application is restarted");
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
                ImGui::PushStyleColor(ImGuiCol_HeaderHovered, ImVec4(0.267f, 0.282f, 0.282f, 1.0f));
                ImGui::PushStyleColor(ImGuiCol_HeaderActive, ImVec4(0.121f, 0.129f, 0.137f, 1.0f));

                if (ImGui::MenuItem(ICON_TEXT(" ", Animation))) { selectedSlate = nullptr; }
                if (ImGui::MenuItem(ICON_TEXT(" ", Audio))) { selectedSlate = nullptr; }
                if (ImGui::MenuItem(ICON_TEXT(" ", Capture Screenshot))) { selectedSlate = nullptr; }
                if (ImGui::MenuItem(ICON_TEXT(" ", Curve Tools))) { selectedSlate = nullptr; }
                if (ImGui::MenuItem(ICON_TEXT(" ", Datetime Format))) { selectedSlate = nullptr; }
                if (ImGui::MenuItem(ICON_TEXT(" ", Display Options))) { selectedSlate = m_ImguiDisplayOptions.get(); }
                if (ImGui::MenuItem(ICON_TEXT(" ", Post Processing))) { selectedSlate = m_ImguiPostProcessing.get(); }
                if (ImGui::MenuItem(ICON_TEXT(" ", Environment))) { selectedSlate = nullptr; }
                if (ImGui::MenuItem(ICON_TEXT(" ", Live))) { selectedSlate = nullptr; }
                if (ImGui::MenuItem(ICON_TEXT(" ", Material))) { selectedSlate = nullptr; }
                if (ImGui::MenuItem(ICON_TEXT(" ", Material Thumbnail))) { selectedSlate = nullptr; }
                if (ImGui::MenuItem(ICON_TEXT(" ", Measure))) { selectedSlate = nullptr; }
                if (ImGui::MenuItem(ICON_TEXT(" ", Metrics Assembler))) { selectedSlate = nullptr; }
                if (ImGui::MenuItem(ICON_TEXT(" ", Navigation))) { selectedSlate = nullptr; }
                if (ImGui::MenuItem(ICON_TEXT(" ", Particle System))) { selectedSlate = nullptr; }
                if (ImGui::MenuItem(ICON_TEXT(" ", Physics))) { selectedSlate = nullptr; }
                if (ImGui::MenuItem(ICON_TEXT(" ", Property Widgets))) { selectedSlate = nullptr; }
                if (ImGui::MenuItem(ICON_TEXT(" ", Rendering))) { selectedSlate = nullptr; }
                if (ImGui::MenuItem(ICON_TEXT(" ", Resource Monitor))) { selectedSlate = nullptr; }
                if (ImGui::MenuItem(ICON_TEXT(" ", Stage))) { selectedSlate = nullptr; }
                if (ImGui::MenuItem(ICON_TEXT(" ", Tagging))) { selectedSlate = nullptr; }
                if (ImGui::MenuItem(ICON_TEXT(" ", Template Startup))) { selectedSlate = nullptr; }
                if (ImGui::MenuItem(ICON_TEXT(" ", Thumbnail Generation))) { selectedSlate = nullptr; }
                if (ImGui::MenuItem(ICON_TEXT(" ", Viewport))) { selectedSlate = nullptr; }
                if (ImGui::MenuItem(ICON_TEXT(" ", Visual Scripting))) { selectedSlate = nullptr; }
                if (ImGui::MenuItem(ICON_TEXT(" ", Visualization))) { selectedSlate = nullptr; }

                ImGui::PopStyleColor(2);
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