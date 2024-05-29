/**
* @file ImguiGizmos.cpp.
* @brief The ImguiGizmos Class Implementation.
* @author Spiecs.
*/

#include "Pchheader.h"
#include "ImguiMainMenu.h"

#include "Systems/SlateSystem.h"
#include "Window/ImguiWindow.h"
#include "Create/ImguiCreateEntity.h"
#include "Help/ImguiHelp.h"

namespace Spiecs {

    ImguiMainMenu::ImguiMainMenu(
        const std::string& panelName , 
        FrameInfo&         frameInfo
    )
        : ImguiSlate(panelName, frameInfo)
    {
        ZoneScoped;

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
        ZoneScoped;

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
                ZoneScopedN("Render Menu File");

                if (ImGui::BeginMenu("File"))
                {
                    ImGui::EndMenu();
                }
            }

            /**
            * @brief Render Menu Edit.
            */
            {
                ZoneScopedN("Render Menu Edit");

                if (ImGui::BeginMenu("Edit"))
                {
                    if (ImGui::MenuItem("Undo", "CTRL+Z")) {}
                    if (ImGui::MenuItem("Redo", "CTRL+Y", false, false)) {}  // Disabled item
                    ImGui::Separator();
                    if (ImGui::MenuItem("Cut", "CTRL+X")) {}
                    if (ImGui::MenuItem("Copy", "CTRL+C")) {}
                    if (ImGui::MenuItem("Paste", "CTRL+V")) {}
                    ImGui::EndMenu();
                }
            }

            /**
            * @brief Render Menu Create.
            */
            {
                ZoneScopedN("Render Menu Create");

                m_Create->OnRender();
            }

            /**
            * @brief Render Menu Window.
            */
            {
                ZoneScopedN("Render Menu Window");

                m_Window->OnRender();
            }

            /**
            * @brief Render Menu Tools.
            */
            {
                ZoneScopedN("Render Menu Tools");

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
                ZoneScopedN("Render Menu Layout");

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
	}
}