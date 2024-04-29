#include "Pchheader.h"
#include "ImguiMainMenu.h"
#include "Resources/ResourcePool/ResourcePool.h"
#include "Resources/Texture/Texture2D.h"
#include "Render/Vulkan/VulkanImage.h"

namespace Spiecs {

    ImguiMainMenu::ImguiMainMenu(const std::string& panelName)
        : ImguiSlate(panelName)
    {
        std::shared_ptr<Texture> t = ResourcePool<Texture>::Load<Texture2D>("street.jpg");
        VkDescriptorImageInfo* info = t->GetResource<VulkanImage>()->GetImageInfo();

        m_ImageID = ImGui_ImplVulkan_AddTexture(info->sampler, info->imageView, info->imageLayout);
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
            if (ImGui::BeginMenu("Create"))
            {
                ImGui::EndMenu();
            }
            BuildWindowMenu();
            if (ImGui::BeginMenu("Tools"))
            {
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Layout"))
            {
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Help"))
            {
                ImGui::EndMenu();
            }
            ImGui::EndMainMenuBar();
        }
	}

    void ImguiMainMenu::BuildWindowMenu()
    {
        if (ImGui::BeginMenu("Window"))
        {
            if (ImGui::BeginMenu("Visualize"))
            {
                ImGui::Image(m_ImageID, ImVec2{ 100, 100 });
                ImGui::Text("Hello");

                ImGui::EndMenu();
            }

            ImGui::EndMenu();
        }
    }
}