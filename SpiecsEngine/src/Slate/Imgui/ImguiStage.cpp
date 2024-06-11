/**
* @file ImguiStage.h
* @brief The ImguiStage Class Implementation.
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
        * @brief Begin render Stage Slate.
        */
        Begin();

        /**
        * @brief Begin render Search Input Text.
        */
        {
            SPIECS_PROFILE_ZONEN("ImguiStage::Search");

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
        }

        /**
        * @brief Begin render entity (tree)list.
        */
        {
            SPIECS_PROFILE_ZONEN("ImguiStage::Entity (tree)list");

            ImGuiTableFlags flags =
                ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable
                | ImGuiTableFlags_Sortable | ImGuiTableFlags_SortMulti
                | ImGuiTableFlags_RowBg | ImGuiTableFlags_Borders | ImGuiTableFlags_NoBordersInBody
                | ImGuiTableFlags_ScrollX | ImGuiTableFlags_ScrollY
                | ImGuiTableFlags_SizingFixedFit;

            ImGuiTableColumnFlags columns_base_flags = ImGuiTableColumnFlags_None;
            int freeze_cols = 1;
            int freeze_rows = 1;
            static ImVector<int> selection;

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

                    const bool item_is_selected = selection.contains((int)entityID);
                    ImGuiSelectableFlags selectable_flags = ImGuiSelectableFlags_SpanAllColumns | ImGuiSelectableFlags_AllowOverlap;

                    std::stringstream ss;
                    ss << ICON_MD_POLYMER << " " << (*tComp.GetTag().begin()).c_str();
                    // todo: multiple select.
                    if (ImGui::Selectable(ss.str().c_str(), item_is_selected, selectable_flags, ImVec2(0, ImGuiH::GetLineItemSize().x)))
                    {
                        if (ImGui::GetIO().KeyCtrl)
                        {
                            if (item_is_selected)
                            {
                                /**
                                * @brief Remove select entity.
                                */

                                selection.find_erase_unsorted((int)entityID);

                                m_FrameInfo.m_PickEntityID.erase((int)entityID);
                            }
                            else
                            {
                                /**
                                * @brief Add select entity.
                                */

                                selection.push_back((int)entityID);

                                m_FrameInfo.m_PickEntityID.push_back((int)entityID, (*tComp.GetTag().begin()));
                            }
                        }
                        else
                        {
                            /**
                            * @brief Set a select entity.
                            */

                            selection.clear();
                            selection.push_back((int)entityID);

                            m_FrameInfo.m_PickEntityID.clear();
                            m_FrameInfo.m_PickEntityID.push_back((int)entityID, (*tComp.GetTag().begin()));
                        }
                    }

                    ImGui::TableSetColumnIndex(1);
                    // todo: hidden entity.
                    if (ImGui::Button(ICON_MD_REMOVE_RED_EYE, ImGuiH::GetLineItemSize()))
                    {

                    }

                    ImGui::TableSetColumnIndex(2);
                    ImGui::Text("Entity");
                });

                ImGui::EndTable();
            }
        }

        /**
        * @brief Begin render add entity panel.
        */
        {
            SPIECS_PROFILE_ZONEN("ImguiStage::Add Entity");

            if (ImGui::BeginPopupContextWindow(0, 1))
            {
                if (ImGui::MenuItem("Create Empty Entity"))
                {
                    std::cout << " Clicked " << std::endl;
                }
                ImGui::EndPopup();
            }
        }

        /**
        * @brief End render Visualizer.
        */
        End();
    }
}