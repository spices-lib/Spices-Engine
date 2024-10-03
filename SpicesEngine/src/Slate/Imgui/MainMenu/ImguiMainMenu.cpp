/**
* @file ImguiGizmos.cpp.
* @brief The ImguiGizmos Class Implementation.
* @author Spices.
*/

#include "Pchheader.h"
#include "ImguiMainMenu.h"

#include "Systems/SlateSystem.h"
#include "ImguiFile.h"
#include "Edit/ImguiEdit.h"
#include "Window/ImguiWindow.h"
#include "Create/ImguiCreateEntity.h"
#include "Help/ImguiHelp.h"

#include <imgui_internal.h>

namespace Spices {

    ImguiMainMenu::ImguiMainMenu(
        const std::string& panelName , 
        FrameInfo&         frameInfo
    )
        : ImguiSlate(panelName, frameInfo)
    {
        SPICES_PROFILE_ZONE;

        /**
        * @brief Instance Child Item.
        */
        {
            m_File   = SlateSystem::GetRegister()->Register<ImguiFile>        (false, "File");
            m_Edit   = SlateSystem::GetRegister()->Register<ImguiEdit>        (false, "Edit");
            m_Create = SlateSystem::GetRegister()->Register<ImguiCreateEntity>(false, "Create");
            m_Window = SlateSystem::GetRegister()->Register<ImguiWindow>      (false, "Window");
            m_Help   = SlateSystem::GetRegister()->Register<ImguiHelp>        (false, "Help");
        }
    }

    void ImguiMainMenu::OnRender()
	{
        SPICES_PROFILE_ZONE;

        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(6.0f, 6.0f));
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(4.0f, 4.0f));
        ImGui::PushItemFlag(ImGuiItemFlags_SelectableDontClosePopup, true);

        /**
        * @brief MainMenu do not need call Begin(), End().
        * ImGui::BeginMainMenuBar() does it automatically.
        */
        if (ImGui::BeginMainMenuBar())
        {
            m_PanelSize = ImGui::GetContentRegionAvail();

            /**
            * @brief Render Menu File.
            */
            m_File->OnRender();
  

            /**
            * @brief Render Menu Edit.
            */
            m_Edit->OnRender();


            /**
            * @brief Render Menu Create.
            */
            m_Create->OnRender();


            /**
            * @brief Render Menu Window.
            */
            m_Window->OnRender();


            /**
            * @brief Render Menu Tools.
            */
            {
                SPICES_PROFILE_ZONEN("Render Menu Tools");

                if (ImGui::BeginMenu("Tools"))
                {
                    ImGuiH::MainMenuTitleSeparator();

                    if (ImGui::BeginMenu(ICON_TEXT(ICON_MD_CHECK, Variants)))
                    {
                        if (ImGui::MenuItem(ICON_TEXT(ICON_MD_CHECK, Variant Editor))) {}
                        if (ImGui::MenuItem(ICON_TEXT(ICON_MD_CHECK, Variant Presenter))) {}

                        ImGui::EndMenu();
                    }
                    if (ImGui::MenuItem(ICON_TEXT(ICON_MD_CHECK, Section))) {}
                    if (ImGui::MenuItem(ICON_TEXT(ICON_MD_CHECK, Paint))) {}
                    if (ImGui::MenuItem(ICON_TEXT(ICON_MD_CHECK, Pivot))) {}
                    if (ImGui::MenuItem(ICON_TEXT(ICON_MD_CHECK, Randomizer))) {}
                    if (ImGui::BeginMenu(ICON_TEXT(ICON_MD_CHECK, Animation)))
                    {
                        if (ImGui::BeginMenu(ICON_TEXT(ICON_MD_CHECK, Curve Processing)))
                        {
                            if (ImGui::MenuItem(ICON_TEXT(ICON_MD_CHECK, Simplify Curve))) {}

                            ImGui::EndMenu();
                        }
                        if (ImGui::BeginMenu(ICON_TEXT(ICON_MD_CHECK, Convert)))
                        {
                            if (ImGui::MenuItem(ICON_TEXT(ICON_MD_CHECK, USD TimeSample to Curves))) {}

                            ImGui::EndMenu();
                        }

                        ImGui::EndMenu();
                    }
                    if (ImGui::MenuItem(ICON_TEXT(ICON_MD_CHECK, Measure))) {}
                    if (ImGui::MenuItem(ICON_TEXT(ICON_MD_CHECK, Array))) {}
                    if (ImGui::MenuItem(ICON_TEXT(ICON_MD_CHECK, Distribute))) {}

                    ImGui::EndMenu();
                }
            }

            /**
            * @brief Render Menu Layout.
            */
            if (ImGui::BeginMenu("Layout"))
            {
                ImGuiH::MainMenuTitleSeparator();

                SPICES_PROFILE_ZONEN("Render Menu Layout");

                if (ImGui::MenuItem("Default", "Ctrl+1")) {}
                if (ImGui::MenuItem("Animation", "Ctrl+2")) {}
                if (ImGui::MenuItem("Animation Graph", "Ctrl+3")) {}
                if (ImGui::MenuItem("Paint", "Ctrl+4")) {}
                if (ImGui::MenuItem("Rendering", "Ctrl+5")) {}
                if (ImGui::MenuItem("Visual Scripting", "Ctrl+6")) {}
                if (ImGui::MenuItem("Physics Authoring", "Ctrl+9")) {}
                ImGui::Separator(); 
                if (ImGui::MenuItem("UI Toggle Visibility", "F7")) {}
                if (ImGui::MenuItem("Fullscreen Mode", "F11")) {}
                ImGui::Separator(); 
                if (ImGui::MenuItem("Save Layout")) {}
                if (ImGui::MenuItem("Load Layout")) {}
                ImGui::Separator(); 
                if (ImGui::MenuItem("Quick Save", "Ctrl+7")) {}
                if (ImGui::MenuItem("Quick Load", "Ctrl+8")) {}

                ImGui::EndMenu();
            }

            /**
            * @brief Render Menu Help.
            */
            m_Help->OnRender();


            /**
            * @brief Render Menu Live.
            */
            {
                SPICES_PROFILE_ZONEN("Render Menu Live");

                ImGui::SameLine(m_PanelSize.x - 6.0f * ImGuiH::GetLineItemSize().x);
                ImGui::Text(ICON_TEXT(ICON_MD_FLASH_ON,LIVE));
                ImGui::Button(ICON_MD_KEYBOARD_ARROW_DOWN, ImGuiH::GetLineItemSize());
                ImGui::Text("CACHE:ON");
            }

            /**
            * @brief End MainMenu;
            */
            ImGui::EndMainMenuBar();
        }

        ImGui::PopItemFlag();
        ImGui::PopStyleVar(2);
	}
}