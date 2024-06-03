/**
* @file ImguiEdit.cpp.
* @brief The ImguiEdit Class Implementation.
* @author Spiecs.
*/

#include "Pchheader.h"
#include "ImguiEdit.h"

namespace Spiecs {

    ImguiEdit::ImguiEdit(
        const std::string& panelName,
        FrameInfo& frameInfo
    )
        : ImguiSlate(panelName, frameInfo)
    {
        SPIECS_PROFILE_ZONE;
    }

    void ImguiEdit::OnRender()
    {
        SPIECS_PROFILE_ZONE;

        /**
        * @brief Begin render ImguiFileMenu.
        */
        if (ImGui::BeginMenu(m_PanelName.c_str()))
        {
            if (ImGui::MenuItem(ICON_TEXT(ICON_EMPTY, Undo), "Ctrl+Z")) {}
            if (ImGui::MenuItem(ICON_TEXT(ICON_EMPTY, Redo), "Ctrl+Y")) {}
            if (ImGui::MenuItem(ICON_TEXT(ICON_EMPTY, Repeat), "Ctrl+R")) {}
            if (ImGui::BeginMenu(ICON_TEXT(ICON_EMPTY, Select)))
            {
                if (ImGui::BeginMenu("Select Recent")) 
                {
                    ImGui::EndMenu();
                }
                if (ImGui::MenuItem("Select All", "Ctrl+A")) {}
                if (ImGui::MenuItem("Select None", "Escape")) {}
                if (ImGui::MenuItem("Select Invert", "Ctrl+i")) {}
                if (ImGui::MenuItem("Select Parent", "Up")) {}
                if (ImGui::MenuItem("Select Leaf")) {}
                if (ImGui::MenuItem("Select Hierarchy")) {}
                if (ImGui::MenuItem("Select Similar")) {}
                ImGui::Separator();
                if (ImGui::MenuItem("Create Selection Set")) {}
                if (ImGui::BeginMenu("Select Set"))
                {
                    ImGui::EndMenu();
                }
                if (ImGui::BeginMenu("Select by Kind"))
                {
                    if (ImGui::MenuItem("Assembly")) {}
                    if (ImGui::MenuItem("Group")) {}
                    if (ImGui::MenuItem("Component")) {}
                    if (ImGui::MenuItem("Subcomponent")) {}
                    ImGui::EndMenu();
                }
                ImGui::EndMenu();
            }
            ImGui::Separator();
            if (ImGui::MenuItem(ICON_TEXT(ICON_EMPTY, Instance)), "Shift+Ctrl+i") {}
            if (ImGui::MenuItem(ICON_TEXT(ICON_MD_CONTROL_POINT_DUPLICATE, Duplicate)), "Ctrl+D") {}
            if (ImGui::MenuItem(ICON_TEXT(ICON_MD_CONTROL_POINT_DUPLICATE, Duplicate-All Layers))) {}
            if (ImGui::MenuItem(ICON_TEXT(ICON_MD_CONTROL_POINT_DUPLICATE, Duplicate-Collapsed))) {}
            if (ImGui::MenuItem(ICON_TEXT(ICON_MD_CONTROL_POINT_DUPLICATE, Parent), "P")) {}
            if (ImGui::MenuItem(ICON_TEXT(ICON_MD_CONTROL_POINT_DUPLICATE, Unparent), "Shift+P")) {}
            if (ImGui::MenuItem(ICON_TEXT(ICON_EMPTY, Group), "Ctrl+G")) {}
            if (ImGui::MenuItem(ICON_TEXT(ICON_EMPTY, Ungroup))) {}
            if (ImGui::MenuItem(ICON_TEXT(ICON_EMPTY, Toggle Visibility), "H")) {}
            ImGui::Separator();
            if (ImGui::MenuItem(ICON_TEXT(ICON_MD_RESTORE_FROM_TRASH, Delete), "Del")) {}
            if (ImGui::MenuItem(ICON_TEXT(ICON_MD_RESTORE_FROM_TRASH, Delete-All Layers))) {}
            if (ImGui::MenuItem(ICON_TEXT(ICON_MD_DRIVE_FILE_RENAME_OUTLINE, Rename), "F2")) {}
            ImGui::Separator();
            if (ImGui::MenuItem(ICON_TEXT(ICON_EMPTY, Hide Unselected))) {}
            if (ImGui::BeginMenu(ICON_TEXT(ICON_EMPTY, Particles)))
            {
                if (ImGui::MenuItem("Restart selected")) {}
                if (ImGui::MenuItem("Restart all")) {}
                if (ImGui::MenuItem("Toggle active selected")) {}
                if (ImGui::MenuItem("Toggle active all")) {}
                ImGui::Separator();
                if (ImGui::MenuItem("Upgrade to latest standards")) {}

                ImGui::EndMenu();
            }
            if (ImGui::MenuItem(ICON_TEXT(ICON_EMPTY, Unhide All))) {}
            ImGui::Separator();
            if (ImGui::MenuItem(ICON_TEXT(ICON_EMPTY, Focus), "F")) {}
            if (ImGui::MenuItem(ICON_TEXT(ICON_EMPTY, Toggle Visualization Mode))) {}
            if (ImGui::MenuItem(ICON_TEXT(ICON_EMPTY, Capture Screenshot), "F10")) {}
            ImGui::Separator();
            if (ImGui::MenuItem(ICON_TEXT(ICON_MD_ROOM_PREFERENCES, Preferences))) {}

            /**
            * @brief End ImguiFileMenu.
            */
            ImGui::EndMenu();
        }
    }
}