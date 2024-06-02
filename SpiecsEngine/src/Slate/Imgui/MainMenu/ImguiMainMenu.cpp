/**
* @file ImguiGizmos.cpp.
* @brief The ImguiGizmos Class Implementation.
* @author Spiecs.
*/

#include "Pchheader.h"
#include "ImguiMainMenu.h"

#include "Systems/SlateSystem.h"
#include "ImguiFile.h"
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
        * @brief Instance a ImguiFile.
        */
        m_File = SlateSystem::GetRegister()->Register<ImguiFile>(false, "File");

        /**
        * @brief Instance a ImguiCreateEntity.
        */
        m_Create = SlateSystem::GetRegister()->Register<ImguiCreateEntity>(false, "Create");

        /**
        * @brief Instance a ImguiWindow.
        */
        m_Window = SlateSystem::GetRegister()->Register<ImguiWindow>(false, "Window");

        /**
        * @brief Instance a ImguiHelp.
        */
        m_Help = SlateSystem::GetRegister()->Register<ImguiHelp>(false, "Help");

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
            {
                SPIECS_PROFILE_ZONEN("Render Menu File");

                if (ImGui::BeginMenu("File"))
                {
                    ImGui::EndMenu();
                }
            }

            /**
            * @brief Render Menu Edit.
            */
            {
                SPIECS_PROFILE_ZONEN("Render Menu Edit");

                m_File->OnRender();
            }

            /**
            * @brief Render Menu Create.
            */
            {
                SPIECS_PROFILE_ZONEN("Render Menu Create");

                m_Create->OnRender();
            }

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