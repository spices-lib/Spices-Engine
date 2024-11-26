/**
* @file ImguiStatistics.cpp.
* @brief The ImguiStatistics Class Implementation.
* @author Spices.
*/

#include "Pchheader.h"
#include "ImguiStatistics.h"
#include "Systems/SlateSystem.h"
#include "Debugger/Perf/NsightPerfGPUProfilerReportGenerator.h"
#include "Debugger/Perf/NsightPerfGPUProfilerOneshotCollection.h"
#include "Debugger/Perf/NsightPerfGPUProfilerContinuous.h"
#include "Core/Library/ProcessLibrary.h"
#include "ImguiVideoMemoryRuntimeHUD.h"
#include "ImguiGPURuntimeProfilerHUD.h"
#include "ImguiRendererProfilerHUD.h"

namespace Spices {

    ImguiStatistics::ImguiStatistics(
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
        m_ImguiRendererProfilerHUD   = SlateSystem::GetRegister()->Register<ImguiRendererProfilerHUD  >(true, "Renderer Profiler HUD");
    }

    void ImguiStatistics::OnRender()
    {
        SPICES_PROFILE_ZONE;

        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.5f, 0.5f, 0.5f, 1.0f));
        ImGui::SeparatorText("GPU Stage");
        ImGui::PopStyleColor();
        if (ImGui::MenuItem(ICON_TEXT(ICON_MD_CHECK, Renderer Profiler HUD), "F8")) { m_ImguiRendererProfilerHUD->SetWindowState(true); }
        if (ImGui::MenuItem(ICON_TEXT(ICON_MD_CHECK, GPU Runtime Profiler HUD), "F9")) { m_ImguiGPURuntimeProfilerHUD->SetWindowState(true); }
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