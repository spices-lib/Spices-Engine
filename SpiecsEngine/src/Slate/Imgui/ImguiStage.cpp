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
        LoadConsoleIcon(m_StageIconID.visibleIcon, "slate/Console.ClearConsole.png");
    }

    void ImguiStage::OnRender()
    {
        Begin();

        char search[128] = "";
        if (ImGui::InputText("Search", search, 128)) {}


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
                ImGui::Text((*tComp.GetTag().begin()).c_str());

                ImGui::TableSetColumnIndex(1);
                ImGui::ImageButton(m_StageIconID.visibleIcon, ImVec2(18, 18));

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

        End();
    }
}