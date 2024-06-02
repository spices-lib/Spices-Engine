/**
* @file ImguiFile.cpp.
* @brief The ImguiFile Class Implementation.
* @author Spiecs.
*/

#include "Pchheader.h"
#include "ImguiFile.h"

namespace Spiecs {

    ImguiFile::ImguiFile(
        const std::string& panelName,
        FrameInfo& frameInfo
    )
        : ImguiSlate(panelName, frameInfo)
    {
        SPIECS_PROFILE_ZONE;
    }

    void ImguiFile::OnRender()
    {
        SPIECS_PROFILE_ZONE;

        /**
        * @brief Begin render ImguiFileMenu.
        */
        if (ImGui::BeginMenu(m_PanelName.c_str()))
        {
            if (ImGui::MenuItem("New", "Ctrl+N")) {}
            if (ImGui::MenuItem("Open", "Ctrl+O")) {}
            if (ImGui::BeginMenu("Open Rencent")) 
            {
                ImGui::EndMenu();
            }
            if (ImGui::MenuItem("Import")) {}
            if (ImGui::BeginMenu("New From Stage Template"))
            {
                ImGui::EndMenu();
            }
            ImGui::Separator();
            if (ImGui::MenuItem("Re-open with New Edit Layer")) {}
            ImGui::Separator();
            if (ImGui::MenuItem("Share", false, false)) {}
            ImGui::Separator();
            if (ImGui::MenuItem("Save", "Ctrl+S")) {}
            if (ImGui::MenuItem("Save With Options", "Ctrl+Alt+S")) {}
            if (ImGui::MenuItem("Save As...", "Shift+Ctrl+S")) {}
            if (ImGui::MenuItem("Save Flattened As...")) {}
            if (ImGui::MenuItem("Export", false, false)) {}
            if (ImGui::MenuItem("Collect As...", false, false)) {}
            ImGui::Separator();
            if (ImGui::MenuItem("Add Reference")) {}
            if (ImGui::MenuItem("Add Payload")) {}
            ImGui::Separator();
            if (ImGui::MenuItem("Exit")) {}

            /**
            * @brief End ImguiFileMenu.
            */
            ImGui::EndMenu();
        }
    }
}