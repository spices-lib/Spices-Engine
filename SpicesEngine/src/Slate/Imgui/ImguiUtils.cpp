/**
* @file ImguiSlate.cpp.
* @brief The ImguiSlate Class Implementation.
* @author Spices.
*/

#include "Pchheader.h"
#include "ImguiUtils.h"

#include "Render/Vulkan/VulkanImage.h"

namespace Spices {

    void ImguiSlate::Begin(float alpha, ImGuiWindowFlags flags)
    {
        Begin(m_PanelName, alpha, flags);
    }

    void ImguiSlate::Begin(const std::string& panelName, float alpha, ImGuiWindowFlags flags)
    {
        SPICES_PROFILE_ZONE;

        // The panel
        if (alpha < 1)
        {
            ImGui::SetNextWindowBgAlpha(alpha);  // For when the panel becomes a floating window
        }

        ImGui::Begin(panelName.c_str(), &m_IsSlateOn, flags);

        if (ImGui::BeginPopupContextItem())
        {
            if (ImGui::MenuItem("Close"))
                m_IsSlateOn = false;
            ImGui::EndPopup();
        }

        /**
        * @brief Query Resize Event Condition.
        */
        QueryIsResizedThisFrame(ImGui::GetContentRegionAvail());

        /**
        * @brief Query Slate State, maybe implementate in parent.
        */
        {
            SPICES_PROFILE_ZONEN("Query Slate State");

            m_PanelPos   = ImGui::GetWindowPos();
            m_IsFocused  = ImGui::IsWindowFocused();
            m_IsHovered  = ImGui::IsWindowHovered();
        }
    }

    void ImguiSlate::End()
    {
        SPICES_PROFILE_ZONE;

        ImGui::End(); 
    }

    void ImguiSlate::LoadSlateIcon(ImTextureID& id, const std::string& iconFile)
    {
        SPICES_PROFILE_ZONE;

        auto rowPtr = ResourcePool<Texture>::Load<Texture2D>(iconFile, iconFile);
        auto info = rowPtr->GetResource<VulkanImage>()->GetImageInfo();

        id = ImGui_ImplVulkan_AddTexture(info->sampler, info->imageView, info->imageLayout);
    }

    void ImguiSlate::QueryIsResizedThisFrame(const ImVec2& thisFrameSize)
    {
        SPICES_PROFILE_ZONE;

        if (m_PanelSize.x != thisFrameSize.x || m_PanelSize.y != thisFrameSize.y)
        {
            m_IsResized = true;
        }
        else
        {
            m_IsResized = false;
        }

        m_PanelSize = thisFrameSize;
    }
}