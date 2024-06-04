/**
* @file ImguiGizmos.cpp.
* @brief The ImguiGizmos Class Implementation.
* @author Spiecs.
*/

#include "Pchheader.h"
#include "ImguiStage.h"

#include "World/World/World.h"

namespace Spiecs {

    ImguiStage::ImguiStage(
        const std::string& panelName , 
        FrameInfo&         frameInfo
    )
        : ImguiSlate(panelName, frameInfo)
    {}

    void ImguiStage::OnRender()
    {
        SPIECS_PROFILE_ZONE;

        /**
        * @brief Begin render Stage.
        */
        Begin();
        ImGui::Separator();
        ImGui::PushItemWidth(m_PanelSize.x - ImGuiH::GetLineItemSize().x * 2.0f);
        char search[128] = "";
        if (ImGui::InputTextWithHint("##", "Search", search, 128)) {}
        ImGui::PopItemWidth();
        
        ImGui::SameLine(m_PanelSize.x - ImGuiH::GetLineItemSize().x * 2.0f);
        ImGui::Button(ICON_MD_FILTER_LIST, ImGuiH::GetLineItemSize());
        ImGui::SameLine(m_PanelSize.x - ImGuiH::GetLineItemSize().x * 1.0f);
        ImGui::Button(ICON_MD_LINE_WEIGHT, ImGuiH::GetLineItemSize());
        ImGui::Separator();

        static ImGuiTableFlags flags =
            ImGuiTableFlags_Resizable  | ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable
            | ImGuiTableFlags_Sortable | ImGuiTableFlags_SortMulti
            | ImGuiTableFlags_RowBg    | ImGuiTableFlags_Borders     | ImGuiTableFlags_NoBordersInBody
            | ImGuiTableFlags_ScrollX  | ImGuiTableFlags_ScrollY
            | ImGuiTableFlags_SizingFixedFit;

        static ImGuiTableColumnFlags columns_base_flags = ImGuiTableColumnFlags_None;
        static int freeze_cols = 1;
        static int freeze_rows = 1;

        if (ImGui::BeginTable("Entity Stage", 3, flags, ImVec2(0, 0), 0.0))
        {
            ImGui::TableSetupColumn("Name", columns_base_flags | ImGuiTableColumnFlags_PreferSortDescending, 0.0f);
            ImGui::TableSetupColumn(ICON_MD_REMOVE_RED_EYE, columns_base_flags | ((flags & ImGuiTableFlags_NoHostExtendX) ? 0 : ImGuiTableColumnFlags_WidthStretch), 0.0f);
            ImGui::TableSetupColumn("Type", columns_base_flags | ImGuiTableColumnFlags_NoSort, 0.0f);
            ImGui::TableSetupScrollFreeze(freeze_cols, freeze_rows);
            ImGui::TableHeadersRow();


            m_FrameInfo.m_World->GetRegistry().view<TagComponent>().each([&](auto entityID, auto& tComp) {
                ImGui::TableNextRow();

                ImGui::TableSetColumnIndex(0);
                ImGui::Text("\xee\xa2\xab ", ImGuiH::GetLineItemSize());
                ImGui::SameLine();
                ImGui::Text((*tComp.GetTag().begin()).c_str());

                ImGui::TableSetColumnIndex(1);
                if (ImGui::Button(ICON_MD_REMOVE_RED_EYE, ImGuiH::GetLineItemSize()));

                ImGui::TableSetColumnIndex(2);
                ImGui::Text("Entity");
            });

            ImGui::EndTable();
        }

        // Right click on blank space
        if (ImGui::BeginPopupContextWindow(0, 1))
        {
            if (ImGui::MenuItem("Create Empty Entity"))
            {
                std::cout << " Clicked " << std::endl;
            }
            ImGui::EndPopup();
        }

        /**
        * @brief End render Visualizer.
        */
        End();
    }
}