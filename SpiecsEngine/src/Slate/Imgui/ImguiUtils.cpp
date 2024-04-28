/**
* @file ImguiSlate.cpp.
* @brief The ImguiSlate Class Implementation.
* @author Spiecs.
*/

#include "Pchheader.h"
#include "ImguiUtils.h"

// Only once.
#include <imgui.cpp>

namespace Spiecs {

    ImGuiID ImguiSlate::dockspaceID;

	void ImguiSlate::BeginMainDock(Side side, float alpha)
	{
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

        // Keeping the unique ID of the dock space
        dockspaceID = ImGui::GetID("DockSpace");

        // The dock need a dummy window covering the entire viewport.
        ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->WorkPos);
        ImGui::SetNextWindowSize(viewport->WorkSize);
        ImGui::SetNextWindowViewport(viewport->ID);
        // All flags to dummy window
        ImGuiWindowFlags host_window_flags = 0;
        host_window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize;
        host_window_flags |= ImGuiWindowFlags_NoMove; //| ImGuiWindowFlags_NoDocking;
        host_window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
        host_window_flags |= ImGuiWindowFlags_NoBackground;
        // Starting dummy window
        char label[32];
        ImFormatString(label, IM_ARRAYSIZE(label), "DockSpaceViewport_%08X", viewport->ID);
        ImGui::Begin(label, NULL, host_window_flags);

        // The central node is transparent, so that when UI is draw after, the image is visible
        // Auto Hide Bar, no title of the panel
        // Center is not dockable, that is for the scene
        ImGuiDockNodeFlags dockspaceFlags = 
            ImGuiDockNodeFlags_PassthruCentralNode | ImGuiDockNodeFlags_AutoHideTabBar | ImGuiDockNodeFlags_NoDockingInCentralNode;

        // Building the splitting of the dock space is done only once
        if (!ImGui::DockBuilderGetNode(dockspaceID))
        {

            ImGui::DockBuilderRemoveNode(dockspaceID);
            ImGui::DockBuilderAddNode(dockspaceID, ImGuiDockNodeFlags_DockSpace);

            ImGuiID dock_main_id = dockspaceID;

            // Slitting all 4 directions
            ImGuiID id_left = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Left, 0.2f, nullptr, &dock_main_id);
            ImGui::DockBuilderDockWindow(side == Side::Left ? m_PanelName.c_str() : "Dock_left", id_left);

            ImGuiID id_right = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Right, 0.2f, nullptr, &dock_main_id);
            ImGui::DockBuilderDockWindow(side == Side::Right ? m_PanelName.c_str() : "Dock_right", id_right);

            ImGuiID id_up = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Up, 0.2f, nullptr, &dock_main_id);
            ImGui::DockBuilderDockWindow(side == Side::Up ? m_PanelName.c_str() : "Dock_up", id_up);

            ImGuiID id_down = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Down, 0.2f, nullptr, &dock_main_id);
            ImGui::DockBuilderDockWindow(side == Side::Down ? m_PanelName.c_str() : "Dock_down", id_down);

            ImGui::DockBuilderDockWindow(side == Side::Scene ? m_PanelName.c_str() : "Scene", dock_main_id);  // Center

            ImGui::DockBuilderFinish(dock_main_id);
        }

        // Setting the panel to blend with alpha
        ImVec4 col = ImGui::GetStyleColorVec4(ImGuiCol_WindowBg);
        ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(col.x, col.y, col.z, alpha));

        ImGui::DockSpace(dockspaceID, ImVec2(0.0f, 0.0f), dockspaceFlags);
        ImGui::PopStyleColor();
        ImGui::End();

        // The panel
        if (alpha < 1)
        {
            ImGui::SetNextWindowBgAlpha(alpha);  // For when the panel becomes a floating window
        }
        ImGui::Begin(m_PanelName.c_str());
	}

    void ImguiSlate::Begin(float alpha)
    {
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

        // The panel
        if (alpha < 1)
        {
            ImGui::SetNextWindowBgAlpha(alpha);  // For when the panel becomes a floating window
        }
        ImGui::Begin(m_PanelName.c_str());
    }
}