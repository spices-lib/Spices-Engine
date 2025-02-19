/**
* @file ImguiStage.h
* @brief The ImguiStage Class Implementation.
* @author Spices.
*/

#include "Pchheader.h"
#include "ImguiStage.h"

#include "Core/Library/StringLibrary.h"
#include "World/World/World.h"
#include "World/Entity.h"
#include "World/Components/EntityComponent.h"
#include "Core/Input/Input.h"
#include "Core/Thread/ThreadModel.h"

namespace Spices {

    ImguiStage::ImguiStage(
        const std::string& panelName , 
        FrameInfo&         frameInfo
    )
        : ImguiSlate(panelName, frameInfo)
    {}

    void ImguiStage::OnRender()
    {
        SPICES_PROFILE_ZONE;

        /**
        * @brief Begin render Stage Slate.
        */
        Begin();

        /**
        * @brief Search String.
        */
        static std::string searchString;
        static bool isEnableSearch = false;

        /**
        * @brief Begin render Search Input Text.
        */
        {
            SPICES_PROFILE_ZONEN("ImguiStage::Search");

            ImGui::Spacing();
            ImGui::Spacing();
            ImGui::PushItemWidth(m_PanelSize.x - ImGuiH::GetLineItemSize().x * 2.0f - ImGui::GetStyle().WindowPadding.x);
            static char search[256] = {};
            if (ImGui::InputTextWithHint("##", ICON_TEXT(ICON_MD_SEARCH, Search), search, 128))
            {
                searchString = std::string(search);
                if (searchString.size() == 0) isEnableSearch = false;
                else isEnableSearch = true;
            }
            ImGui::PopItemWidth();

            ImGui::SameLine(m_PanelSize.x - ImGuiH::GetLineItemSize().x * 2.0f);
            ImGui::Button(ICON_MD_FILTER_ALT, ImGuiH::GetLineItemSize());
            ImGui::SameLine(m_PanelSize.x - ImGuiH::GetLineItemSize().x * 1.0f);
            ImGui::Button(ICON_MD_REORDER, ImGuiH::GetLineItemSize());
            ImGui::Spacing();
        }

        /**
        * @brief Begin render entity (tree)list.
        */
        {
            SPICES_PROFILE_ZONEN("ImguiStage::Entity (tree)list");

            static ImGuiTableFlags flags = ImGuiTableFlags_BordersV | ImGuiTableFlags_BordersOuterH | ImGuiTableFlags_Resizable | ImGuiTableFlags_NoBordersInBody |
                                           ImGuiTableFlags_Sortable | ImGuiTableFlags_Reorderable   | ImGuiTableFlags_SortMulti | ImGuiTableFlags_Hideable |
                                           ImGuiTableFlags_ScrollY  | ImGuiTableFlags_SortTristate;
            static constexpr ImGuiSelectableFlags selectable_flags = ImGuiSelectableFlags_SpanAllColumns | ImGuiSelectableFlags_AllowOverlap;

            /**
            * @brief 0: NoSort, 1: Ascending, 2: Descending.
            */
            static int sortState = 0;

            static std::function<void(bool, Entity, uint32_t)> ClickSelect = [&](bool selected, Entity& entity, uint32_t e) {
            
                if (ImGui::IsItemHovered() && ImGui::IsItemClicked(ImGuiMouseButton_Left))
                {
                    auto& tagComp = entity.GetComponent<TagComponent>();

                    if (ImGui::GetIO().KeyShift)
                    {
                        if (!selected)
                        {
                            /**
                            * @brief Add select entity.
                            */
                            m_FrameInfo.m_PickEntityID.push_back((int)e, (*tagComp.GetTag().begin()));
                        }
                    }
                    else if (ImGui::GetIO().KeyCtrl)
                    {
                        if (selected)
                        {
                            /**
                            * @brief Remove select entity.
                            */
                            m_FrameInfo.m_PickEntityID.erase((int)e);
                        }
                    }
                    else
                    {
                        /**
                        * @brief Set a select entity.
                        */
                        m_FrameInfo.m_PickEntityID.clear();
                        m_FrameInfo.m_PickEntityID.push_back((int)e, (*tagComp.GetTag().begin()));
                    }
                }
            };

            static std::function<void(uint32_t, uint32_t, uint32_t)> DragDrop = [&](uint32_t e, uint32_t p, uint32_t depth) {
            
                if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_AcceptNoDrawDefaultRect))
                {
                    ImGui::SetDragDropPayload("Stage_Drag", &e, sizeof(e), ImGuiCond_Once);
                    ImGui::EndDragDropSource();

                    if (depth > 0)
                    {
                        AsyncTask(ThreadPoolEnum::Game, [=]() {

                            /**
                            * @brief remove source from source's parent.
                            */
                            Entity parent((entt::entity)p, FrameInfo::Get().m_World.get());
                            EntityComponent& comp = parent.GetComponent<EntityComponent>(); 
                            comp.RemoveEntity(e);
                            if (comp.GetEntities().empty())
                            {
                                parent.RemoveComponent<EntityComponent>();
                            }
                        });
                    }
                }
                if (ImGui::BeginDragDropTarget())
                {
                    if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("Stage_Drag"))
                    {
                        uint32_t sourceEntity = *(uint32_t*)payload->Data;

                        AsyncTask(ThreadPoolEnum::Game, [=]() {

                            /**
                            * @brief Add source to this EntityComponent.
                            */
                            Entity((entt::entity)e, FrameInfo::Get().m_World.get()).AddComponent<EntityComponent>().AddEntity(sourceEntity);

                        });
                    }
                    ImGui::EndDragDropTarget();
                }
            };

            static std::function<void(uint32_t, uint32_t, uint32_t)> DrawStageTree = [&](uint32_t e, uint32_t p, uint32_t depth) {

                Entity entity((entt::entity)e, FrameInfo::Get().m_World.get());

                auto& tagComp = entity.GetComponent<TagComponent>();
                bool hasChild = entity.HasComponent<EntityComponent>();

                /**
                * @brief Search Filter here.
                */
                if (isEnableSearch)
                {
                    if ((*tagComp.GetTag().begin()).find(searchString) == std::string::npos)
                    {
                        if (hasChild)
                        {
                            auto& entityComp = entity.GetComponent<EntityComponent>();
                            for (auto& child : entityComp.GetEntities())
                            {
                                DrawStageTree(child, e, depth + 1);
                            }
                        }

                        return;
                    }
                }

                std::stringstream ss;
                ss << (*tagComp.GetTag().begin()).c_str() << "##" << (uint32_t)entity;

                ImGui::PushID(ss.str().c_str());

                ImGui::TableNextRow();
                ImGui::TableNextColumn();

                ImGuiTreeNodeFlags tree_node_flags = 0;
                const bool item_is_selected = m_FrameInfo.m_PickEntityID.has_key((int)e);
                if (item_is_selected)
                {
                    tree_node_flags |= ImGuiTreeNodeFlags_Selected;
                }

                std::stringstream space;
                for (int i = 0; i < depth; i++)
                {
                    space << "  ";
                }
                ImGui::Text(space.str().c_str());
                ImGui::SameLine();

                if (hasChild)
                {
                    bool open = ImGui::TreeNodeEx(ss.str().c_str(), tree_node_flags);
                    ClickSelect(item_is_selected, entity, e);
                    DragDrop(e, p, depth);
                    ImGui::TableNextColumn();
                    ImGui::Button(ICON_MD_REMOVE_RED_EYE, ImGuiH::GetLineItemSize());
                    ImGui::TableNextColumn();
                    ImGui::Text("Entity");
                    if (open)
                    {
                        auto& entityComp = entity.GetComponent<EntityComponent>();
                        for (auto& child : entityComp.GetEntities())
                        {
                            DrawStageTree(child, e, depth + 1);
                        }

                        ImGui::TreePop();
                    }
                }
                else
                {
                    ImGui::TreeNodeEx(ss.str().c_str(), tree_node_flags | ImGuiTreeNodeFlags_NoTreePushOnOpen | ImGuiTreeNodeFlags_Leaf);
                    ClickSelect(item_is_selected, entity, e);
                    DragDrop(e, p, depth);
                    ImGui::TableNextColumn();
                    ImGui::Button(ICON_MD_REMOVE_RED_EYE, ImGuiH::GetLineItemSize());
                    ImGui::TableNextColumn();
                    ImGui::Text("Entity");
                }

                ImGui::PopID();
            };

            ImGui::PushStyleVar(ImGuiStyleVar_IndentSpacing, 0.0f);
            ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(0.196f, 0.204f, 0.2f, 1.0f));
            ImGui::PushStyleColor(ImGuiCol_HeaderHovered, ImVec4(0.164f, 0.18f, 0.184f, 1.0f));
            ImGui::PushStyleColor(ImGuiCol_HeaderActive, ImVec4(0.164f, 0.18f, 0.184f, 1.0f));
            ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.196f, 0.204f, 0.2f, 1.0f));

            if (ImGui::BeginTable("EntityTree", 3, flags))
            {
                // The first column will use the default _WidthStretch when ScrollX is Off and _WidthFixed when ScrollX is On
                ImGui::TableSetupColumn("Name", ImGuiTableColumnFlags_NoHide);
                ImGui::TableSetupColumn(ICON_MD_REMOVE_RED_EYE, ImGuiTableColumnFlags_WidthFixed | ImGuiTableColumnFlags_NoSort | ImGuiTableColumnFlags_NoResize, ImGuiH::GetLineItemSize().x);
                ImGui::TableSetupColumn("Type", ImGuiTableColumnFlags_WidthFixed | ImGuiTableColumnFlags_NoSort | ImGuiTableColumnFlags_NoResize, ImGuiH::GetLineItemSize().x * 3.0f);
                ImGui::TableHeadersRow();

                // Sort our data if sort specs have been changed!
                if (ImGuiTableSortSpecs* sort_specs = ImGui::TableGetSortSpecs())
                {
                    // State changed since last frame.
                    if (sort_specs->SpecsDirty)
                    {
                        sortState = 0;

                        // Need sort this frame.
                        if (sort_specs->SpecsCount > 0)
                        {
                            sortState = sort_specs->Specs->SortDirection;
                        }

                        sort_specs->SpecsDirty = false;
                    }
                }

                FrameInfo::Get().m_World->ViewRoot([&](Entity& entity) {
                    DrawStageTree(entity, 0, 0);
                });

                /**
                * @brief Begin render add entity panel.
                */
                {
                    SPICES_PROFILE_ZONEN("ImguiStage::Add Entity");

                    if (ImGui::BeginPopupContextWindow(0, ImGuiPopupFlags_MouseButtonRight))
                    {
                        if (ImGui::MenuItem("Create Empty Entity"))
                        {
                            AsyncTask(ThreadPoolEnum::Game, [=]() {

                                FrameInfo::Get().m_World->CreateEntity("Entity");

                            });
                        }
                        ImGui::EndPopup();
                    }
                }

                ImGui::EndTable();
            }
            ImGui::PopStyleColor(4);
            ImGui::PopStyleVar();
        }

        /**
        * @brief End render Visualizer.
        */
        End();
    }
}