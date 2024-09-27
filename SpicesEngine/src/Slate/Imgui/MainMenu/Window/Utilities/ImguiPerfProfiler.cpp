/**
* @file ImguiPerfProfiler.cpp.
* @brief The ImguiPerfProfiler Class Implementation.
* @author Spices.
*/

#include "Pchheader.h"
#include "ImguiPerfProfiler.h"
#include "Debugger/Perf/NsightPerf.h"
#include "Render/Vulkan/VulkanRenderBackend.h"

namespace Spices {

    ImguiPerfProfiler::ImguiPerfProfiler(
        const std::string& panelName ,
        FrameInfo&         frameInfo
    )
        : ImguiSlate(panelName, frameInfo)
    {}

    void ImguiPerfProfiler::OnRender()
    {
        SPICES_PROFILE_ZONE;

        if (!m_IsSlateOn) return;

        /**
        * @brief Begin render PerfProfilerHUD.
        */
        Begin("Graphics General Triage");

        PERF_RENDERHUD;

        /**
        * @brief End render GBuffer Visualizer.
        */
        End();
    }
}