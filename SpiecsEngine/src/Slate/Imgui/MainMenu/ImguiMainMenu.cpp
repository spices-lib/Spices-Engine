#include "Pchheader.h"
#include "ImguiMainMenu.h"
#include "Systems/SlateSystem.h"

#include "Window/ImguiWindow.h"
#include "Create/ImguiCreateEntity.h"
#include "Help/ImguiHelp.h"

namespace Spiecs {

    ImguiMainMenu::ImguiMainMenu(const std::string& panelName)
        : ImguiSlate(panelName)
    {
        m_Create = SlateSystem::GetRegister()->Register<ImguiCreateEntity>(false, "Create");
        m_Window = SlateSystem::GetRegister()->Register<ImguiWindow>(false, "Window");
        m_Help = SlateSystem::GetRegister()->Register<ImguiHelp>(false, "Help");
    }

    void ImguiMainMenu::OnRender()
	{
        /**
        * @brief MainMenu do not need call Begin(), End().
        * ImGui::BeginMainMenuBar() does it automatically.
        */
        if (ImGui::BeginMainMenuBar())
        {
            if (ImGui::BeginMenu("File"))
            {
                ImGui::EndMenu();
            }
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
            m_Create->OnRender();
            m_Window->OnRender();
            if (ImGui::BeginMenu("Tools"))
            {
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Layout"))
            {
                ImGui::EndMenu();
            }
            m_Help->OnRender();
            ImGui::EndMainMenuBar();
        }
	}
}