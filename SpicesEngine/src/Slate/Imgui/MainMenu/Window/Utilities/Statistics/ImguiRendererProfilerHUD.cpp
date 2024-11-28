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

        RendererManager::IterRenderer([&](const std::string& rendererName, const std::shared_ptr<Renderer>& renderer) {
            
            renderer->IterStatistics([&](const std::string& subPassName, const std::shared_ptr<RenderPassStatistics>& statistics) {
                
                statistics->IterStatisticsResult([&](const Queryer::StatisticsBits& type, std::shared_ptr<Queryer::Result>& result) {
                    
                    if (type == Queryer::Timestamp && (1 << selectedQueryer) == Queryer::Timestamp)
                    {
                        TimestampQueryer::Result* res = static_cast<TimestampQueryer::Result*>(result.get());
                        if(res->valid) ImGui::Text(std::to_string(res->timeStamp).c_str());
                    }

                    if (type == Queryer::Pipeline && (1 << selectedQueryer) == Queryer::Pipeline)
                    {
                        PipelineStatisticsQueryer::Result* res = static_cast<PipelineStatisticsQueryer::Result*>(result.get());
                        ImGui::SeparatorText(subPassName.c_str());
                        if (res->valid)
                        {
                            for (int i = 0; i < (int)PipelineStatisticEnum::MAX; i++)
                            {
                                ImGui::Text(PipelineStatisticEnumToString((PipelineStatisticEnum)i).c_str());
                                ImGui::SameLine(300.0f);
                                ImGui::Text(std::to_string(res->statistics[i]).c_str());
                            }
                        }
                        ImGui::Spacing();
                    }
                });
                return false;
            });

            return false;
        });

        /**
        * @brief End render GBuffer Visualizer.
        */
        End();
    }
}