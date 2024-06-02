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
            if (ImGui::MenuItem("Undo", "Ctrl+Z")) {}
            if (ImGui::MenuItem("Redo", "Ctrl+Y")) {}
            if (ImGui::MenuItem("Repeat", "Ctrl+R")) {}
            if (ImGui::BeginMenu("Select"))
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
            if (ImGui::MenuItem("Instance"), "Shift+Ctrl+i") {}
            if (ImGui::MenuItem("Duplicate"), "Ctrl+D") {}
            if (ImGui::MenuItem("Duplicate-All Layers")) {}
            if (ImGui::MenuItem("Duplicate-Collapsed")) {}
            if (ImGui::MenuItem("Parent", "P")) {}
            if (ImGui::MenuItem("Unparent", "Shift+P")) {}
            if (ImGui::MenuItem("Group", "Ctrl+G")) {}
            if (ImGui::MenuItem("Ungroup")) {}
            if (ImGui::MenuItem("Toggle Visibility", "H")) {}
            ImGui::Separator();
            if (ImGui::MenuItem("Delete", "Del")) {}
            if (ImGui::MenuItem("Delete-All Layers")) {}
            if (ImGui::MenuItem("Rename", "F2")) {}
            ImGui::Separator();
            if (ImGui::MenuItem("Hide Unselected")) {}
            if (ImGui::BeginMenu("Particles"))
            {
                if (ImGui::MenuItem("Restart selected")) {}
                if (ImGui::MenuItem("Restart all")) {}
                if (ImGui::MenuItem("Toggle active selected")) {}
                if (ImGui::MenuItem("Toggle active all")) {}
                ImGui::Separator();
                if (ImGui::MenuItem("Upgrade to latest standards")) {}

                ImGui::EndMenu();
            }
            if (ImGui::MenuItem("Unhide All")) {}
            ImGui::Separator();
            if (ImGui::MenuItem("Focus", "F")) {}
            if (ImGui::MenuItem("Toggle Visualization Mode")) {}
            if (ImGui::MenuItem("Capture Screenshot", "F10")) {}
            ImGui::Separator();
            if (ImGui::MenuItem("Preferences")) {}

            /**
            * @brief End ImguiFileMenu.
            */
            ImGui::EndMenu();
        }
    }
}