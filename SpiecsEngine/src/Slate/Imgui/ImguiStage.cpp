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
    {
        ZoneScoped;

        /**
        * @brief Load used icon from file.
        */
        LoadSlateIcon(m_StageIconID.filterIcon,    "slate/stage/Stage.Filter.png"   );
        LoadSlateIcon(m_StageIconID.optionsIcon,   "slate/stage/Stage.Options.png"  );

        LoadSlateIcon(m_StageIconID.visibleIcon,   "slate/stage/Stage.Visible.png"  );
        LoadSlateIcon(m_StageIconID.invisibleIcon, "slate/stage/Stage.Invisible.png");

        LoadSlateIcon(m_StageIconID.entityIcon,    "slate/stage/Stage.MeshComponent.png"   );
    }

    void ImguiStage::OnRender()
    {
        ZoneScoped;

        /**
        * @brief Begin render Stage.
        */
        Begin();
        ImGui::Separator();
        char search[128] = "";
        if (ImGui::InputTextWithHint("##", "Search", search, 128)) {}
        
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0.0f, 0.0f));
        ImGui::SameLine(m_PanelSize.x - m_LineHeight * 2.4f);
        ImGui::ImageButton(m_StageIconID.filterIcon, ImVec2(m_LineHeight, m_LineHeight));
        ImGui::SameLine(m_PanelSize.x - m_LineHeight * 1.2f);
        ImGui::ImageButton(m_StageIconID.optionsIcon, ImVec2(m_LineHeight, m_LineHeight));
        ImGui::PopStyleVar();
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
            ImGui::TableSetupColumn("Visable", columns_base_flags | ((flags & ImGuiTableFlags_NoHostExtendX) ? 0 : ImGuiTableColumnFlags_WidthStretch), 0.0f);
            ImGui::TableSetupColumn("Type", columns_base_flags | ImGuiTableColumnFlags_NoSort, 0.0f);
            ImGui::TableSetupScrollFreeze(freeze_cols, freeze_rows);
            ImGui::TableHeadersRow();


            m_FrameInfo.m_World->GetRegistry().view<TagComponent>().each([&](auto entityID, auto& tComp) {
                ImGui::TableNextRow();

                ImGui::TableSetColumnIndex(0);
                ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0.0f, 0.0f));
                ImGui::Image(m_StageIconID.entityIcon, ImVec2(m_LineHeight, m_LineHeight));
                ImGui::PopStyleVar();
                ImGui::SameLine();
                ImGui::Text((*tComp.GetTag().begin()).c_str());

                ImGui::TableSetColumnIndex(1);
                ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0.0f, 0.0f));
                ImTextureID icon = m_StageIconID.visibleIcon;
                if (ImGui::ImageButton(icon, ImVec2(m_LineHeight, m_LineHeight)));
                ImGui::PopStyleVar();

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