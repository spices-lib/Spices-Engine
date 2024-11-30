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
#include "Render/Renderer/RenderPassStatistics/TimestampQueryer.h"
#include "Render/Renderer/RenderPassStatistics/PipelineStatisticsQueryer.h"
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
        * @brief Begin render PerfProfilerHUD.
        */
        Begin();

        if (ImGui::Button("Refresh", ImVec2(100.0f, 100.0f)))
        {
            RENDERPASS_STATISTICS_CAPTUREFRAME
        }

        static const char* queryer[] = { "TimeStamp", "PipelineStatistics" };
        static int selectedQueryer = 0;
        ImGui::Combo("##", &selectedQueryer, queryer, _countof(queryer));

        if (selectedQueryer == 0)
        {
            DrawTimeStamp();
        }
        else if(selectedQueryer == 1)
        {
            DrawPipelineStatistics();
        }

        /**
        * @brief End render GBuffer Visualizer.
        */
        End();
    }

    void ImguiRendererProfilerHUD::DrawTimeStamp()
    {
        SPICES_PROFILE_ZONE;

        struct TimestampResult
        {
            TimestampQueryer::Result result;
            std::string name;
        };

        scl::tree<TimestampResult> totalResult;
        totalResult.GetData().name = "Scene";

        {
            SPICES_PROFILE_ZONEN("Fetch Statistics Caches");

            RendererManager::IterRenderer([&](const std::string& rendererName, const std::shared_ptr<Renderer>& renderer) {

                auto rendererResult = totalResult.AddChild();
                rendererResult->GetData().name = rendererName;

                renderer->IterStatistics([&](const std::string& subPassName, const std::shared_ptr<RenderPassStatistics>& statistics) {
                
                    auto subPassResult = rendererResult->AddChild();
                    subPassResult->GetData().name = subPassName;

                    statistics->IterStatisticsResult(Queryer::Timestamp, [&](const Queryer::StatisticsBits& type, std::shared_ptr<Queryer::Result>& result) {

                        TimestampQueryer::Result* res = static_cast<TimestampQueryer::Result*>(result.get());
                        if (res->valid)
                        {
                            subPassResult->GetData().result = *res;
                            rendererResult->GetData().result.Combine(res);
                            totalResult.GetData().result.Combine(res);
                        }
                    });

                    return false;
                });

                return false;
            });
        }



        ImGuiH::DrawTreeProgressBar("Scene", [&]() {

            std::stringstream ss;
            ss << " " << std::setprecision(3) << totalResult.GetData().result.timeStamp << " ms";

            ImGui::ProgressBar(totalResult.GetData().result.timeStamp > 0.01f ? 1.0f : 0.0f, ImVec2(-FLT_MIN, 2.0f), "##");
            ImGui::SameLine(ImGui::GetContentRegionAvail().x * 0.3f);
            ImGui::Text(ss.str().c_str());

        },  [&]() {

            for(auto& rendererStatistics : totalResult.GetChilds())
            {
                TimestampResult& rendererRes = rendererStatistics->GetData();

                ImGuiH::DrawTreeProgressBar(rendererRes.name.c_str(), [&]() {
                    
                    std::stringstream ss;
                    ss << " " << std::setprecision(3) << rendererRes.result.timeStamp << " ms";

                    ImGui::ProgressBar(totalResult.GetData().result.timeStamp > 0.01f ? rendererRes.result.timeStamp / totalResult.GetData().result.timeStamp : 0.0f, ImVec2(-FLT_MIN, 2.0f), "##");
                    ImGui::SameLine(ImGui::GetContentRegionAvail().x * 0.3f);
                    ImGui::Text(ss.str().c_str());

                }, [&]() {

                    for (auto& subPassStatistics : rendererStatistics->GetChilds())
                    {
                        TimestampResult& subPassRes = subPassStatistics->GetData();

                        ImGui::Text(subPassRes.name.c_str());
                        
                        if (subPassRes.result.valid)
                        {
                            ImGui::SameLine((ImGui::GetContentRegionAvail().x - ImGui::GetStyle().FramePadding.x) * 0.3f);

                            std::stringstream ss;
                            ss << " " << std::setprecision(3) << subPassRes.result.timeStamp << " ms";

                            ImGui::ProgressBar(totalResult.GetData().result.timeStamp > 0.01f ? glm::min(1.0f, subPassRes.result.timeStamp / totalResult.GetData().result.timeStamp) : 0.0f, ImVec2(-FLT_MIN, 2.0f), "##");
                            ImGui::SameLine(ImGui::GetContentRegionAvail().x * 0.3f);
                            ImGui::Text(ss.str().c_str());
                        }
                    }
                });
            }
        });

    }

    void ImguiRendererProfilerHUD::DrawPipelineStatistics()
    {
        /*TimestampQueryer::Result totalResult;

        renderer->IterStatistics([&](const std::string& subPassName, const std::shared_ptr<RenderPassStatistics>& statistics) {
                
            TimestampQueryer::Result rendererResult;

            statistics->IterStatisticsResult([&](const Queryer::StatisticsBits& type, std::shared_ptr<Queryer::Result>& result) {
                    
                if (type == Queryer::Timestamp && (1 << selectedQueryer) == Queryer::Timestamp)
                {
                    ImGui::Text(subPassName.c_str());
                        
                    TimestampQueryer::Result* res = static_cast<TimestampQueryer::Result*>(result.get());
                    if (res->valid)
                    {
                        ImGui::SameLine(300.0f);
                        ImGui::Text(std::to_string(res->timeStamp).c_str());
                    }
                }

                if (type == Queryer::Pipeline && (1 << selectedQueryer) == Queryer::Pipeline)
                {
                    PipelineStatisticsQueryer::Result* res = static_cast<PipelineStatisticsQueryer::Result*>(result.get());
                    ImGui::SeparatorText(subPassName.c_str());
                    if (res->valid)
                    {
                        for (int i = 0; i < (int)PipelineStatisticEnum::MAX; i++)
                        {
                            if (res->statistics[i] > 0)
                            {
                                ImGui::Text(PipelineStatisticEnumToString((PipelineStatisticEnum)i).c_str());
                                ImGui::SameLine(300.0f);
                                ImGui::Text(std::to_string(res->statistics[i]).c_str());
                            }
                        }
                    }
                    ImGui::Spacing();
                }
            });
            return false;
        });*/
    }
}