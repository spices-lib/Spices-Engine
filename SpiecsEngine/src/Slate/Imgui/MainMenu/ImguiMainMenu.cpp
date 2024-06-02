/**
* @file ImguiGizmos.cpp.
* @brief The ImguiGizmos Class Implementation.
* @author Spiecs.
*/

#include "Pchheader.h"
#include "ImguiMainMenu.h"

#include "Systems/SlateSystem.h"
#include "ImguiFile.h"
#include "ImguiEdit.h"
#include "Window/ImguiWindow.h"
#include "Create/ImguiCreateEntity.h"
#include "Help/ImguiHelp.h"

#include <imgui_internal.h>

namespace Spiecs {

    ImguiMainMenu::ImguiMainMenu(
        const std::string& panelName , 
        FrameInfo&         frameInfo
    )
        : ImguiSlate(panelName, frameInfo)
    {
        SPIECS_PROFILE_ZONE;

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
        SPIECS_PROFILE_ZONE;

        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(4.0f, 4.0f));
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(6.0f, 6.0f));
        ImGui::PushItemFlag(ImGuiItemFlags_SelectableDontClosePopup, true);

        /**
        * @brief MainMenu do not need call Begin(), End().
        * ImGui::BeginMainMenuBar() does it automatically.
        */
        if (ImGui::BeginMainMenuBar())
        {
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
            {
                SPIECS_PROFILE_ZONEN("Render Menu Window");

                m_Window->OnRender();
            }

            /**
            * @brief Render Menu Tools.
            */
            {
                SPIECS_PROFILE_ZONEN("Render Menu Tools");

                if (ImGui::BeginMenu("Tools"))
                {
                    ImGui::EndMenu();
                }
            }

            /**
            * @brief Render Menu Layout.
            */
            if (ImGui::BeginMenu("Layout"))
            {
                SPIECS_PROFILE_ZONEN("Render Menu Layout");

                ImGui::EndMenu();
            }

            /**
            * @brief Render Menu Help.
            */
            m_Help->OnRender();

            /**
            * @brief End MainMenu;
            */
            ImGui::EndMainMenuBar();
        }

        ImGui::PopItemFlag();
        ImGui::PopStyleVar(2);
	}
}