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

        RendererManager::IterRenderer([&](const std::string& rendererName, const std::shared_ptr<Renderer>& renderer) {
            
            renderer->IterStatistics([&](const std::string& subPassName, const std::shared_ptr<RenderPassStatistics>& statistics) {
                
                statistics->IterStatisticsResult([&](const Queryer::StatisticsBits& type, std::shared_ptr<Queryer::Result>& result) {
                    
                    if (type == Queryer::Timestamp)
                    {
                        TimestampQueryer::Result* res = static_cast<TimestampQueryer::Result*>(result.get());
                        ImGui::Text(std::to_string(res->timeStamp).c_str());
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