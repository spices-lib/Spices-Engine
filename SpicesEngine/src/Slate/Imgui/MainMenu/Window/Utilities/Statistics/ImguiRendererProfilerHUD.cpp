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
        static std::unordered_map<std::string, TimestampQueryer::Result> statisticsCaches;

        TimestampQueryer::Result totalResult = statisticsCaches["Scene"];
        statisticsCaches["Scene"].timeStamp = 0.0f;

        ImGuiH::DrawTreeProgressBar("Scene", [&]() {

            std::stringstream ss;
            ss << totalResult.timeStamp << " ms";

            ImGui::ProgressBar(totalResult.timeStamp > 0.01f ? 1.0f : 0.0f, ImVec2(-FLT_MIN, 2.0f), ss.str().c_str());

        },  [&]() {

            RendererManager::IterRenderer([&](const std::string& rendererName, const std::shared_ptr<Renderer>& renderer) {

                TimestampQueryer::Result rendererResult = statisticsCaches[rendererName];
                statisticsCaches[rendererName].timeStamp = 0.0f;

                ImGuiH::DrawTreeProgressBar(rendererName.c_str(), [&]() {
                    
                    std::stringstream ss;
                    ss << rendererResult.timeStamp << " ms";

                    ImGui::ProgressBar(totalResult.timeStamp > 0.01f ? rendererResult.timeStamp / totalResult.timeStamp : 0.0f, ImVec2(-FLT_MIN, 2.0f), ss.str().c_str());

                }, [&]() {

                    renderer->IterStatistics([&](const std::string& subPassName, const std::shared_ptr<RenderPassStatistics>& statistics) {
                
                        statistics->IterStatisticsResult(Queryer::Timestamp, [&](const Queryer::StatisticsBits& type, std::shared_ptr<Queryer::Result>& result) {
                    
                            ImGui::Text(subPassName.c_str());
                        
                            TimestampQueryer::Result* res = static_cast<TimestampQueryer::Result*>(result.get());
                            if (res->valid)
                            {
                                ImGui::SameLine((ImGui::GetContentRegionAvail().x - ImGui::GetStyle().FramePadding.x) * 0.3f);

                                std::stringstream ss;
                                ss << res->timeStamp << " ms";

                                ImGui::ProgressBar(totalResult.timeStamp > 0.01f ? glm::min(1.0f, res->timeStamp / totalResult.timeStamp) : 0.0f, ImVec2(-FLT_MIN, 2.0f), ss.str().c_str());

                                statisticsCaches[rendererName].Combine(res);
                                statisticsCaches["Scene"].Combine(res);
                            }
                        });

                        return false;
                    });

                });

                return false;
            });

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