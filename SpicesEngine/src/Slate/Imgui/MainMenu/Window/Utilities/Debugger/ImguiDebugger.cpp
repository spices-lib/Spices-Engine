/**
* @file ImguiGPUProfilerHUD.cpp.
* @brief The ImguiGPUProfilerHUD Class Implementation.
* @author Spices.
*/

#include "Pchheader.h"
#include "ImguiDebugger.h"
#include "Systems/SlateSystem.h"
#include "Debugger/Perf/NsightPerfGPUProfilerReportGenerator.h"
#include "Debugger/Perf/NsightPerfGPUProfilerOneshotCollection.h"
#include "Core/Library/ProcessLibrary.h"
#include "ImguiVideoMemoryRuntimeHUD.h"
#include "ImguiGPURuntimeProfilerHUD.h"

namespace Spices {

    ImguiDebugger::ImguiDebugger(
        const std::string& panelName ,
        FrameInfo&         frameInfo
    )
        : ImguiSlate(panelName, frameInfo)
    {
        SPICES_PROFILE_ZONE;

        /**
        * @brief Instance.
        */
        m_ImguiVideoMemoryRuntimeHUD = SlateSystem::GetRegister()->Register<ImguiVideoMemoryRuntimeHUD>(true, "Video Memory Runtime HUD");
        m_ImguiGPURuntimeProfilerHUD = SlateSystem::GetRegister()->Register<ImguiGPURuntimeProfilerHUD>(true, "GPU Runtime Profiler HUD");
    }

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
        if (ImGui::MenuItem(ICON_TEXT(ICON_MD_CHECK, GPU Runtime Profiler HUD), "F9")) { m_ImguiGPURuntimeProfilerHUD->SetWindowState(true); }
        if (ImGui::MenuItem(ICON_TEXT(ICON_MD_CHECK, GPU Report Generate), "F10")) { NSIGHTPERF_GPUPROFILERREPORT_CAPTUREFRAME }
        if (ImGui::MenuItem(ICON_TEXT(ICON_MD_CHECK, GPU One-Shot Collection), "F10")) { NSIGHTPERF_GPUPROFILERONESHOT_CAPTUREFRAME }
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.5f, 0.5f, 0.5f, 1.0f));
        ImGui::SeparatorText("Video Memory Stage");
        ImGui::PopStyleColor();
        if (ImGui::MenuItem(ICON_TEXT(ICON_MD_CHECK, Video Memory Runtime HUD), "F11")) { m_ImguiVideoMemoryRuntimeHUD->SetWindowState(true); }

        /**
        * @brief End render Utilities.
        */
        ImGui::EndMenu();
    }
}