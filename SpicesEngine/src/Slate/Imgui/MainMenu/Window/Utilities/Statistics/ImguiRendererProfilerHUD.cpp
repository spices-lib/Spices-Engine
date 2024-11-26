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
                
                statistics->GetStatisticsResult();
                statistics->DrawStatisticsResult();

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