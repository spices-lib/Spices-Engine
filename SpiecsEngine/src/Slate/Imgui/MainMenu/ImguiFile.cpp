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
            if (ImGui::MenuItem(ICON_TEXT(ICON_MD_NEW_LABEL, New), "Ctrl+N")) {}
            if (ImGui::MenuItem(ICON_TEXT(ICON_MD_FOLDER_OPEN, Open), "Ctrl+O")) {}
            if (ImGui::BeginMenu(ICON_TEXT(ICON_EMPTY, Open Rencent)))
            {
                ImGui::EndMenu();
            }
            if (ImGui::MenuItem(ICON_TEXT(ICON_EMPTY, Import))) {}
            if (ImGui::BeginMenu(ICON_TEXT(ICON_MD_NEW_LABEL, New From Stage Template)))
            {
                ImGui::EndMenu();
            }
            ImGui::Separator();
            if (ImGui::MenuItem(ICON_TEXT(ICON_MD_OPEN_IN_NEW, Re-open with New Edit Layer))) {}
            ImGui::Separator();
            if (ImGui::MenuItem(ICON_TEXT(ICON_MD_SHARE, Share), false, false)) {}
            ImGui::Separator();
            if (ImGui::MenuItem(ICON_TEXT(ICON_MD_SAVE, Save), "Ctrl+S")) {}
            if (ImGui::MenuItem(ICON_TEXT(ICON_MD_SAVE, Save With Options), "Ctrl+Alt+S")) {}
            if (ImGui::MenuItem(ICON_TEXT(ICON_EMPTY, Save As...), "Shift+Ctrl+S")) {}
            if (ImGui::MenuItem(ICON_TEXT(ICON_EMPTY, Save Flattened As...))) {}
            if (ImGui::MenuItem(ICON_TEXT(ICON_EMPTY, Export), false, false)) {}
            if (ImGui::MenuItem(ICON_TEXT(ICON_EMPTY, Collect As...), false, false)) {}
            ImGui::Separator();
            if (ImGui::MenuItem(ICON_TEXT(ICON_EMPTY, Add Reference))) {}
            if (ImGui::MenuItem(ICON_TEXT(ICON_EMPTY, Add Payload))) {}
            ImGui::Separator();
            if (ImGui::MenuItem(ICON_TEXT(ICON_EMPTY, Exit))) {}

            /**
            * @brief End ImguiFileMenu.
            */
            ImGui::EndMenu();
        }
    }
}