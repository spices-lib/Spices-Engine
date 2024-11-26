/**
* @file ImguiDebugger.cpp.
* @brief The ImguiDebugger Class Implementation.
* @author Spices.
*/

#include "Pchheader.h"
#include "ImguiDebugger.h"
#include "Systems/SlateSystem.h"
#include "Debugger/Perf/NsightPerfGPUProfilerReportGenerator.h"
#include "Debugger/Perf/NsightPerfGPUProfilerOneshotCollection.h"
#include "Debugger/Perf/NsightPerfGPUProfilerContinuous.h"
#include "Core/Library/ProcessLibrary.h"

namespace Spices {

    ImguiDebugger::ImguiDebugger(
        const std::string& panelName ,
        FrameInfo&         frameInfo
    )
        : ImguiSlate(panelName, frameInfo)
    {}

    void ImguiDebugger::OnRender()
    {
        SPICES_PROFILE_ZONE;

        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.5f, 0.5f, 0.5f, 1.0f));
        ImGui::SeparatorText("CPU Stage");
        ImGui::PopStyleColor();
        if (ImGui::MenuItem(ICON_TEXT(ICON_MD_CHECK, CPU Profiler), "F8")) { ProcessLibrary::OpenProcess(m_ProfileProcessName.c_str()); }
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.5f, 0.5f, 0.5f, 1.0f));
        ImGui::SeparatorText("GPU Stage");
        ImGui::PopStyleColor();
        if (ImGui::MenuItem(ICON_TEXT(ICON_MD_CHECK, GPU Continuous), "F9")) { NSIGHTPERF_GPUPROFILERCONTINUOUS_CAPTUREFRAME }
        if (ImGui::MenuItem(ICON_TEXT(ICON_MD_CHECK, GPU Report Generate), "F10")) { NSIGHTPERF_GPUPROFILERREPORT_CAPTUREFRAME }
        if (ImGui::MenuItem(ICON_TEXT(ICON_MD_CHECK, GPU One-Shot Collection), "F10")) { NSIGHTPERF_GPUPROFILERONESHOT_CAPTUREFRAME }

        /**
        * @brief End render Utilities.
        */
        ImGui::EndMenu();
    }
}