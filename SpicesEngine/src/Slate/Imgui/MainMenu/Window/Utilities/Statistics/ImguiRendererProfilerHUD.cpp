 /**
* @file ImguiRendererProfilerHUD.cpp.
* @brief The ImguiRendererProfilerHUD Class Implementation.
* @author Spices.
*/

#include "Pchheader.h"
#include "ImguiRendererProfilerHUD.h"
#include "Render/Renderer/RendererManager.h"
#include "Render/Renderer/Renderer.h"
#include "Render/Renderer/RenderPassStatistics/RenderPassStatistics.h"
#include "..\..\..\..\..\..\Render\Renderer\RenderPassStatistics\TimestampQuerier.h"
#include "..\..\..\..\..\..\Render\Renderer\RenderPassStatistics\PipelineStatisticsQuerier.h"
#include "Core/Container/Tree.h"

namespace Spices {

    ImguiRendererProfilerHUD::ImguiRendererProfilerHUD(
        const std::string& panelName ,
        FrameInfo&         frameInfo
    )
        : ImguiSlate(panelName, frameInfo)
    {}

    void ImguiRendererProfilerHUD::OnRender()
    {
        SPICES_PROFILE_ZONE;

        if (!m_IsSlateOn) return;

        /**
        * @brief Begin render RendererProfilerHUD.
        */
        Begin();
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(2.0f, 4.0f));

        /**
        * @brief Search String.
        */
        static std::string searchString;
        static bool isEnableSearch = false;

        /**
        * @brief Begin render Search Input Text.
        */
        {
            SPICES_PROFILE_ZONEN("ImguiRendererProfilerHUD::Search");

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

        static int selectedStatistics = 0;
        static int selectedChannel = 0;

        ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(0.196f, 0.204f, 0.2f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_HeaderHovered, ImVec4(0.164f, 0.18f, 0.184f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_HeaderActive, ImVec4(0.164f, 0.18f, 0.184f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.196f, 0.204f, 0.2f, 1.0f));

        /**
        * @brief Render Datails.
        */
        {
            SPICES_PROFILE_ZONEN("ImguiRendererProfilerHUD::Datails");

            {
                ImGui::PushID("ImguiRendererProfilerHUD::Statistics Type");
                ImGui::Columns(2, 0, false);
                ImGui::SetColumnWidth(0, ImGuiH::GetLineItemSize().x * 4.0f);
                ImGui::Text("Statistics");
                ImGui::NextColumn();

                static const char* type[] = { " TimeStamp", " PipelineStatistics" };
                ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x);
                ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.6f, 0.6f, 0.6f, 1.0f));
                ImGui::Combo("##", &selectedStatistics, type, _countof(type));
                ImGui::PopStyleColor();
                ImGui::PopItemWidth();

                ImGui::Columns(1);
                ImGui::PopID();
            }

            if (selectedStatistics == 1)
            {
                ImGui::PushID("ImguiRendererProfilerHUD::PipelineChannel");
                ImGui::Columns(2, 0, false);
                ImGui::SetColumnWidth(0, ImGuiH::GetLineItemSize().x * 4.0f);
                ImGui::Text("Channel");
                ImGui::NextColumn();

                static const char* channel[] = { 
                    " Input Assembly Vertices", 
                    " Input Assembly Primitive",
                    " Vertex Shader Invocations",
                    " Geometry Shader Invocations",
                    " Geometry Shader Primitive",
                    " Clipping Invocations",
                    " Clipping Primitive",
                    " Fragment Shader Invocations",
                    " Tessellation Control Invocations",
                    " Tessellation Evaluation Invocations",
                    " Compute Shader Invocations",
                    " Task Shader Invocations",
                    " Mesh Shader Invocations"
                };
                ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x);
                ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.6f, 0.6f, 0.6f, 1.0f));
                ImGui::Combo("##", &selectedChannel, channel, _countof(channel));
                ImGui::PopStyleColor();
                ImGui::PopItemWidth();

                ImGui::Columns(1);
                ImGui::PopID();
            }

            {
                ImGui::PushID("ImguiRendererProfilerHUD::Refresh");
                ImGui::Columns(2, 0, false);
                ImGui::SetColumnWidth(0, ImGuiH::GetLineItemSize().x * 4.0f);
                ImGui::Text("Refresh");
                ImGui::NextColumn();

                static bool isChecked = false;
                ImGuiH::Checkbox(&isChecked);

                if (isChecked)
                {
                    RENDERPASS_STATISTICS_CAPTUREFRAME
                    isChecked = false;
                }

                ImGui::Columns(1);
                ImGui::PopID();
            }
        }

        ImGui::PopStyleColor(4);

        if (selectedStatistics == 0)
        {
            DrawTimeStamp();
        }
        else if(selectedStatistics == 1)
        {
            DrawPipelineStatistics(selectedChannel);
        }

        /**
        * @brief End render RendererProfilerHUD Visualizer.
        */
        ImGui::PopStyleVar();
        End();
    }

    void ImguiRendererProfilerHUD::DrawTimeStamp() const
    {
        SPICES_PROFILE_ZONE;

        /**
        * @brief Tree node.
        */
        struct TimestampResult
        {
            TimestampQuerier::Result result;
            std::string name;
        };

        /**
        * @brief Instance a Tree.
        */
        scl::tree<TimestampResult> totalResult;
        totalResult.GetData().name = "Scene ( Viewport 0 )";

        /**
        * @brief Write scene data to tree.
        */
        {
            SPICES_PROFILE_ZONEN("Fetch Statistics Caches");

            RendererManager::IterRenderer([&](const std::string& rendererName, const std::shared_ptr<Renderer>& renderer) {

                const auto rendererResult = totalResult.AddChild();
                rendererResult->GetData().name = rendererName;

                renderer->IterStatistics([&](const std::string& subPassName, const std::shared_ptr<RenderPassStatistics>& statistics) {
                
                    const auto subPassResult = rendererResult->AddChild();
                    subPassResult->GetData().name = subPassName;

                    statistics->IterStatisticsResult(Querier::Timestamp, [&](const Querier::StatisticsBits& type, std::shared_ptr<Querier::Result>& result) {

                        TimestampQuerier::Result* res = static_cast<TimestampQuerier::Result*>(result.get());
                        if (res->valid)
                        {
                            subPassResult->GetData().result = *res;

                            rendererResult->GetData().result.Combine(res);
                            rendererResult->GetData().result.valid = true;

                            totalResult.GetData().result.Combine(res);
                            totalResult.GetData().result.valid = true;
                        }
                    });

                    return false;
                });

                return false;
            });
        }

        {
            SPICES_PROFILE_ZONEN("Draw Statistics Caches");

            static ImGuiTableFlags flags = ImGuiTableFlags_BordersV | ImGuiTableFlags_BordersOuterH | ImGuiTableFlags_Resizable | ImGuiTableFlags_NoBordersInBody | 
                                           ImGuiTableFlags_Sortable | ImGuiTableFlags_Reorderable | ImGuiTableFlags_SortMulti | ImGuiTableFlags_Hideable |
                                           ImGuiTableFlags_ScrollY | ImGuiTableFlags_SortTristate;
            static ImGuiTreeNodeFlags tree_node_flags = ImGuiTreeNodeFlags_SpanAllColumns;

            /**
            * @brief 0: NoSort, 1: Ascending, 2: Descending.
            */
            static int sortState = 0;

            /**
            * @brief Function of quick sort to PipelineResult.
            */
            static auto SortTimestampTreeMethod = [](const void* lhs, const void* rhs) -> int {
                
                const auto l = static_cast<const std::unique_ptr<scl::tree<TimestampResult>>*>(lhs);
                const auto r = static_cast<const std::unique_ptr<scl::tree<TimestampResult>>*>(rhs);

                ImGuiTableSortSpecs* sort_specs = ImGui::TableGetSortSpecs();
                if (sortState == ImGuiSortDirection_Ascending)
                {
                    return ((*l)->GetData().result.timeStamp - (*r)->GetData().result.timeStamp) * 1000;
                }
                else
                {
                    return ((*r)->GetData().result.timeStamp - (*l)->GetData().result.timeStamp) * 1000;
                }
            };

            /**
            * @brief Function of sort whole tree.
            */
            static std::function<void(scl::tree<TimestampResult>*)> SortTimestampTree = [&](scl::tree<TimestampResult>* node) {

                if (sortState == 0) return;

                qsort((void*)node->GetChilds().data(), (size_t)node->GetChilds().size(), sizeof(node->GetChilds()[0]), SortTimestampTreeMethod);

                for (auto& child : node->GetChilds())
                {
                    SortTimestampTree(child.get());
                }
            };

            /**
            * @brief Function of draw whole tree.
            */
            static std::function<void(scl::tree<TimestampResult>*, uint32_t, scl::tree<TimestampResult>*)> DrawTimestampTree = [&](scl::tree<TimestampResult>* node, uint32_t depth, scl::tree<TimestampResult>* root) {
            
                if (!node->GetData().result.valid) return;

                ImGui::TableNextRow();
                ImGui::TableNextColumn();

                std::stringstream space;
                for (uint32_t i = 0; i < depth; i++)
                {
                    space << " ";
                }

                ImGui::Text(space.str().c_str());
                ImGui::SameLine();

                bool hasChild = node->GetChilds().size() > 0;
                if (hasChild)
                {
                    bool open = ImGui::TreeNodeEx(node->GetData().name.c_str(), tree_node_flags);
                    ImGui::TableNextColumn();

                    ImGui::PushStyleColor(ImGuiCol_PlotHistogram, ImVec4(0.0f, 0.8f, 0.0f, 1.0f));
                    ImGui::ProgressBar(root->GetData().result.timeStamp > 0 ? (node->GetData().result.timeStamp / root->GetData().result.timeStamp) : 0.0f, ImVec2(-FLT_MIN, 2.0f), "##");
                    ImGui::PopStyleColor();
                    ImGui::SameLine(0.05f);
                    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.8f, 0.8f, 0.8f, 1.0f));
                    ImGui::Text("%.2f ms", node->GetData().result.timeStamp);
                    ImGui::PopStyleColor();
                    if (open)
                    {
                        for (auto& child : node->GetChilds())
                        {
                            DrawTimestampTree(child.get(), depth + 1, root);
                        }
                        ImGui::TreePop();
                    }
                }
                else
                {
                    ImGui::TreeNodeEx(node->GetData().name.c_str(), tree_node_flags | ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen);
                    ImGui::TableNextColumn();
                         
                    ImGui::PushStyleColor(ImGuiCol_PlotHistogram, ImVec4(0.0f, 0.8f, 0.0f, 1.0f));
                    ImGui::ProgressBar(root->GetData().result.timeStamp > 0 ? (node->GetData().result.timeStamp / root->GetData().result.timeStamp) : 0.0f, ImVec2(-FLT_MIN, 2.0f), "##");
                    ImGui::PopStyleColor();
                    ImGui::SameLine(0.05f);
                    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.8f, 0.8f, 0.8f, 1.0f));
                    ImGui::Text("%.2f ms", node->GetData().result.timeStamp);
                    ImGui::PopStyleColor();
                }
            };

            ImGui::PushStyleVar(ImGuiStyleVar_IndentSpacing, 0.0f);
            ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(0.196f, 0.204f, 0.2f, 1.0f));
            ImGui::PushStyleColor(ImGuiCol_HeaderHovered, ImVec4(0.164f, 0.18f, 0.184f, 1.0f));
            ImGui::PushStyleColor(ImGuiCol_HeaderActive, ImVec4(0.164f, 0.18f, 0.184f, 1.0f));
            ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.196f, 0.204f, 0.2f, 1.0f));

            if (ImGui::BeginTable("timestampTree", 2, flags))
            {
                // The first column will use the default _WidthStretch when ScrollX is Off and _WidthFixed when ScrollX is On
                ImGui::TableSetupColumn("Name", ImGuiTableColumnFlags_NoHide);
                ImGui::TableSetupColumn("TimeCost", ImGuiTableColumnFlags_WidthFixed | ImGuiTableColumnFlags_PreferSortDescending, ImGuiH::GetLineItemSize().x * 3.0f);
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

                SortTimestampTree(&totalResult);
                DrawTimestampTree(&totalResult, 0, &totalResult);

                ImGui::EndTable();
            }
            ImGui::PopStyleColor(4);
            ImGui::PopStyleVar();
        }
    }

    void ImguiRendererProfilerHUD::DrawPipelineStatistics(int bit) const
    {
        SPICES_PROFILE_ZONE;

        /**
        * @brief Store bit as static variable.
        */
        static int selectBit;
        selectBit = bit;

        /**
        * @brief Tree node.
        */
        struct PipelineResult
        {
            PipelineStatisticsQuerier::Result result;
            std::string name;
        };

        /**
        * @brief Instance a Tree.
        */
        scl::tree<PipelineResult> totalResult;
        totalResult.GetData().name = "Scene ( Viewport 0 )";

        /**
        * @brief Write scene data to tree.
        */
        {
            SPICES_PROFILE_ZONEN("Fetch Statistics Caches");

            RendererManager::IterRenderer([&](const std::string& rendererName, const std::shared_ptr<Renderer>& renderer) {

                auto rendererResult = totalResult.AddChild();
                rendererResult->GetData().name = rendererName;

                renderer->IterStatistics([&](const std::string& subPassName, const std::shared_ptr<RenderPassStatistics>& statistics) {
                
                    auto subPassResult = rendererResult->AddChild();
                    subPassResult->GetData().name = subPassName;

                    statistics->IterStatisticsResult(Querier::Pipeline, [&](const Querier::StatisticsBits& type, std::shared_ptr<Querier::Result>& result) {

                        PipelineStatisticsQuerier::Result* res = static_cast<PipelineStatisticsQuerier::Result*>(result.get());
                        if (res->valid)
                        {
                            subPassResult->GetData().result = *res;

                            rendererResult->GetData().result.Combine(res);
                            rendererResult->GetData().result.valid = true;

                            totalResult.GetData().result.Combine(res);
                            totalResult.GetData().result.valid = true;
                        }
                    });

                    return false;
                });

                return false;
            });
        }

        {
            SPICES_PROFILE_ZONEN("Draw Statistics Caches");

            static ImGuiTableFlags flags = ImGuiTableFlags_BordersV | ImGuiTableFlags_BordersOuterH | ImGuiTableFlags_Resizable | ImGuiTableFlags_NoBordersInBody | 
                                           ImGuiTableFlags_Sortable | ImGuiTableFlags_Reorderable | ImGuiTableFlags_SortMulti | ImGuiTableFlags_Hideable |
                                           ImGuiTableFlags_ScrollY | ImGuiTableFlags_SortTristate;
            static ImGuiTreeNodeFlags tree_node_flags = ImGuiTreeNodeFlags_SpanAllColumns;

            /**
            * @brief 0: NoSort, 1: Ascending, 2: Descending.
            */
            static int sortState = 0;

            /**
            * @brief Function of quick sort to PipelineResult.
            */
            static auto SortTimestampTreeMethod = [](const void* lhs, const void* rhs) -> int {
                
                const auto l = static_cast<const std::unique_ptr<scl::tree<PipelineResult>>*>(lhs);
                const auto r = static_cast<const std::unique_ptr<scl::tree<PipelineResult>>*>(rhs);

                ImGuiTableSortSpecs* sort_specs = ImGui::TableGetSortSpecs();
                if (sortState == ImGuiSortDirection_Ascending)
                {
                    return (*l)->GetData().result.statistics[selectBit] - (*r)->GetData().result.statistics[selectBit];
                }
                else
                {
                    return (*r)->GetData().result.statistics[selectBit] - (*l)->GetData().result.statistics[selectBit];
                }
            };

            /**
            * @brief Function of sort whole tree.
            */
            static std::function<void(scl::tree<PipelineResult>*)> SortTimestampTree = [&](scl::tree<PipelineResult>* node) {

                if (sortState == 0) return;

                qsort((void*)node->GetChilds().data(), (size_t)node->GetChilds().size(), sizeof(node->GetChilds()[0]), SortTimestampTreeMethod);

                for (auto& child : node->GetChilds())
                {
                    SortTimestampTree(child.get());
                }
            };

            /**
            * @brief Function of draw whole tree.
            */
            static std::function<void(scl::tree<PipelineResult>*, uint32_t depth, scl::tree<PipelineResult>*)> DrawTimestampTree = [&](scl::tree<PipelineResult>* node, uint32_t depth, scl::tree<PipelineResult>* root) {
            
                if (!node->GetData().result.valid) return;

                ImGui::TableNextRow();
                ImGui::TableNextColumn();

                std::stringstream space;
                for (uint32_t i = 0; i < depth; i++)
                {
                    space << " ";
                }

                ImGui::Text(space.str().c_str());
                ImGui::SameLine();

                bool hasChild = node->GetChilds().size() > 0;
                if (hasChild)
                {
                    bool open = ImGui::TreeNodeEx(node->GetData().name.c_str(), tree_node_flags);
                    ImGui::TableNextColumn();

                    ImGui::PushStyleColor(ImGuiCol_PlotHistogram, ImVec4(0.0f, 0.8f, 0.0f, 1.0f));
                    ImGui::ProgressBar(root->GetData().result.statistics[bit] > 0 ? (node->GetData().result.statistics[bit] / float(root->GetData().result.statistics[bit])) : 0.0f, ImVec2(-FLT_MIN, 2.0f), "##");
                    ImGui::PopStyleColor();
                    ImGui::SameLine(0.05f);
                    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.8f, 0.8f, 0.8f, 1.0f));
                    ImGui::Text("%d", node->GetData().result.statistics[bit]);
                    ImGui::PopStyleColor();
                    if (open)
                    {
                        for (auto& child : node->GetChilds())
                        {
                            DrawTimestampTree(child.get(), depth + 1, root);
                        }
                        ImGui::TreePop();
                    }
                }
                else
                {
                    ImGui::TreeNodeEx(node->GetData().name.c_str(), tree_node_flags | ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen);
                    ImGui::TableNextColumn();
                         
                    ImGui::PushStyleColor(ImGuiCol_PlotHistogram, ImVec4(0.0f, 0.8f, 0.0f, 1.0f));
                    ImGui::ProgressBar(root->GetData().result.statistics[bit] > 0 ? (node->GetData().result.statistics[bit] / float(root->GetData().result.statistics[bit])) : 0.0f, ImVec2(-FLT_MIN, 2.0f), "##");
                    ImGui::PopStyleColor();
                    ImGui::SameLine(0.05f);
                    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.8f, 0.8f, 0.8f, 1.0f));
                    ImGui::Text("%d", node->GetData().result.statistics[bit]);
                    ImGui::PopStyleColor();
                }
            };

            ImGui::PushStyleVar(ImGuiStyleVar_IndentSpacing, 0.0f);
            ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(0.196f, 0.204f, 0.2f, 1.0f));
            ImGui::PushStyleColor(ImGuiCol_HeaderHovered, ImVec4(0.164f, 0.18f, 0.184f, 1.0f));
            ImGui::PushStyleColor(ImGuiCol_HeaderActive, ImVec4(0.164f, 0.18f, 0.184f, 1.0f));
            ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.196f, 0.204f, 0.2f, 1.0f));

            if (ImGui::BeginTable("pipelineTree", 2, flags))
            {
                // The first column will use the default _WidthStretch when ScrollX is Off and _WidthFixed when ScrollX is On
                ImGui::TableSetupColumn("Name", ImGuiTableColumnFlags_NoSort | ImGuiTableColumnFlags_NoHide);
                ImGui::TableSetupColumn("Invocation Count", ImGuiTableColumnFlags_WidthFixed | ImGuiTableColumnFlags_PreferSortDescending, ImGuiH::GetLineItemSize().x * 3.0f);
                ImGui::TableSetupScrollFreeze(0, 1);
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

                SortTimestampTree(&totalResult);
                DrawTimestampTree(&totalResult, 0, &totalResult);

                ImGui::EndTable();
            }
            ImGui::PopStyleColor(4);
            ImGui::PopStyleVar();
        }
    }
}