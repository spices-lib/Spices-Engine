/**
* @file ImguiUtilities.cpp.
* @brief The ImguiUtilities Class Implementation.
* @author Spices.
*/

#include "Pchheader.h"
#include "ImguiUtilities.h"
#include "Debugger/Perf/NsightPerfReportGenerator.h"

#include "Systems/SlateSystem.h"
#include "Core/Library/ProcessLibrary.h"
#include "ImguiGPUProfilerHUD.h"

namespace Spices {

    ImguiUtilities::ImguiUtilities(
        const std::string& panelName ,
        FrameInfo&         frameInfo
    )
        : ImguiSlate(panelName, frameInfo)
    {
        SPICES_PROFILE_ZONE;

        /**
        * @brief Instance.
        */
        m_ImguiGPUProfilerHUD = SlateSystem::GetRegister()->Register<ImguiGPUProfilerHUD>(true, "GPU Profiler HUD");
    }

    void ImguiUtilities::OnRender()
    {
        SPICES_PROFILE_ZONE;

        /**
        * @brief Begin render Utilities.
        */
        std::stringstream ss;
        ss << ICON_EMPTY << " " << m_PanelName.c_str();
        if (ImGui::BeginMenu(ss.str().c_str()))
        {
            /**
            * @brief Render Profiler.
            */
            if (ImGui::MenuItem(ICON_TEXT(ICON_MD_CHECK, Actions))) {}
            if (ImGui::MenuItem(ICON_TEXT(ICON_MD_CHECK, Activity Progress))) {}
            if (ImGui::MenuItem(ICON_TEXT(ICON_MD_CHECK, Asset Validator))) {}
            if (ImGui::MenuItem(ICON_TEXT(ICON_MD_CHECK, Console))) {}
            if (ImGui::MenuItem(ICON_TEXT(ICON_MD_CHECK, TracyProfiler), "F8")) { ProcessLibrary::OpenProcess(m_ProfileProcessName.c_str()); }
            if (ImGui::MenuItem(ICON_TEXT(ICON_MD_CHECK, GPU Profiler HUD), "F9"))  { m_ImguiGPUProfilerHUD->SetWindowState(true); }
            if (ImGui::MenuItem(ICON_TEXT(ICON_MD_CHECK, CaptureFrame), "F10")) { PERFREPORT_ENABLECAPTURE }
            if (ImGui::MenuItem(ICON_TEXT(ICON_MD_CHECK, Scene Optimizer))) {}
            if (ImGui::MenuItem(ICON_TEXT(ICON_MD_CHECK, Statistics))) {}
            if (ImGui::MenuItem(ICON_TEXT(ICON_MD_CHECK, USD Paths))) {}

            /**
            * @brief End render Utilities.
            */
            ImGui::EndMenu();
        }
    }
}