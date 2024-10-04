/**
* @file ImguiGPUMemoryHUD.cpp.
* @brief The ImguiGPUMemoryHUD Class Implementation.
* @author Spices.
*/

#include "Pchheader.h"
#include "ImguiGPUMemoryHUD.h"
#include "Debugger/Perf/NsightGPUProfilerHUD.h"
#include "Render/Vulkan/VulkanRenderBackend.h"

namespace Spices {

    ImguiGPUMemoryHUD::ImguiGPUMemoryHUD(
        const std::string&  panelName ,
        FrameInfo&          frameInfo
    )
        : ImguiSlate(panelName, frameInfo)
    {}

    void ImguiGPUMemoryHUD::OnRender()
    {
        SPICES_PROFILE_ZONE;

        if (!m_IsSlateOn) return;

        /**
        * @brief Instance a VkPhysicalDeviceMemoryBudgetPropertiesEXT.
        */
        VkPhysicalDeviceMemoryBudgetPropertiesEXT      memoryBudgetProperties{};
        memoryBudgetProperties.sType                 = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MEMORY_BUDGET_PROPERTIES_EXT;
        memoryBudgetProperties.pNext                 = nullptr;

        /**
        * @brief Instance a VkPhysicalDeviceMemoryProperties2.
        */
        VkPhysicalDeviceMemoryProperties2              memoryProperties{};
        memoryProperties.sType                       = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MEMORY_PROPERTIES_2;
        memoryProperties.pNext                       = &memoryBudgetProperties;

        /**
        * @brief Query memory data this frame.
        */
        vkGetPhysicalDeviceMemoryProperties2(VulkanRenderBackend::GetState().m_PhysicalDevice, &memoryProperties);

        uint64_t nHeaps                              = memoryProperties.memoryProperties.memoryHeapCount;
        VkDeviceSize usage                           = 0;
        VkDeviceSize budget                          = 0;

        for (uint64_t i = 0; i < nHeaps; i++)
        {
            usage                                   += memoryBudgetProperties.heapUsage[i];
            budget                                  += memoryBudgetProperties.heapBudget[i];
        }

        /**
        * @brief Begin render PerfProfilerHUD.
        */
        Begin();

        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(2.0f, 4.0f));

        /**
        * @brief Search String.
        */
        static std::string searchString;
        static bool isEnableSearch = false;

        /**
        * @brief Begin render Search Input Text.
        */
        {
            SPICES_PROFILE_ZONEN("ImguiGPUMemoryHUD::Search");

            ImGui::Spacing();
            ImGui::PushItemWidth(m_PanelSize.x - ImGuiH::GetLineItemSize().x * 2.0f - ImGui::GetStyle().WindowPadding.x);
            static char search[256] = {};
            if (ImGui::InputTextWithHint("##", ICON_TEXT(ICON_MD_SEARCH, Search), search, 128))
            {
                searchString = std::string(search);
                if (searchString.size() == 0) isEnableSearch = false;
                else isEnableSearch = true;
            }
            ImGui::PopItemWidth();

            ImGui::SameLine(m_PanelSize.x - ImGuiH::GetLineItemSize().x * 2.0f);
            ImGui::Button(ICON_MD_FILTER_ALT, ImGuiH::GetLineItemSize());
            ImGui::SameLine(m_PanelSize.x - ImGuiH::GetLineItemSize().x * 1.0f);
            ImGui::Button(ICON_MD_REORDER, ImGuiH::GetLineItemSize());
            ImGui::Spacing();
        }

        float columeWidth = ImGuiH::GetLineItemSize().x * 6.5f;

        {
            SPICES_PROFILE_ZONEN("ImguiGPUMemoryHUD::Total");

            ImGuiH::DrawTreeTitle("Memory Total", nullptr, [&]() { 
                ImGuiH::DrawPropertyItem("Usage:", columeWidth, [&]() {
                    ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x - ImGuiH::GetLineItemSize().x);
                    char buffer[256] = {};
                    ImGui::InputTextWithHint("##", ConvertBytestoString(usage).c_str(), buffer, sizeof(buffer));
                    ImGui::PopItemWidth();
                    ImGui::SameLine();
                    ImGuiH::DrawResetIcon(false);
                });
                ImGuiH::DrawPropertyItem("Budget:", columeWidth, [&]() {
                    ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x - ImGuiH::GetLineItemSize().x);
                    char buffer[256] = {};
                    ImGui::InputTextWithHint("##", ConvertBytestoString(budget).c_str(), buffer, sizeof(buffer));
                    ImGui::PopItemWidth();
                    ImGui::SameLine();
                    ImGuiH::DrawResetIcon(false);
                });
                ImGuiH::DrawPropertyItem("Heap Count:", columeWidth, [&]() {
                    ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x - ImGuiH::GetLineItemSize().x);
                    char buffer[256] = {};
                    ImGui::InputTextWithHint("##", std::to_string(nHeaps).c_str(), buffer, sizeof(buffer));
                    ImGui::PopItemWidth();
                    ImGui::SameLine();
                    ImGuiH::DrawResetIcon(false);
                });
            });
        }
        
        {
            SPICES_PROFILE_ZONEN("ImguiGPUMemoryHUD::Heaps");

            for (int i = 0; i < nHeaps; i++)
            {
                std::stringstream s;
                s << "Memory Heap Index: " << i;

                ImGuiH::DrawTreeTitle(s.str(), nullptr, [&]() {
                    ImGuiH::DrawPropertyItem("Usage:", columeWidth, [&]() {
                        ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x - ImGuiH::GetLineItemSize().x);
                        char buffer[256] = {};
                        ImGui::InputTextWithHint("##", ConvertBytestoString(memoryBudgetProperties.heapUsage[i]).c_str(), buffer, sizeof(buffer));
                        ImGui::PopItemWidth();
                        ImGui::SameLine();
                        ImGuiH::DrawResetIcon(false);
                    });
                    ImGuiH::DrawPropertyItem("Budget:", columeWidth, [&]() {
                        ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x - ImGuiH::GetLineItemSize().x);
                        char buffer[256] = {};
                        ImGui::InputTextWithHint("##", ConvertBytestoString(memoryBudgetProperties.heapBudget[i]).c_str(), buffer, sizeof(buffer));
                        ImGui::PopItemWidth();
                        ImGui::SameLine();
                        ImGuiH::DrawResetIcon(false);
                    });
                    ImGuiH::DrawPropertyItem("Heap Count:", columeWidth, [&]() {
                        ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x - ImGuiH::GetLineItemSize().x);
                        char buffer[256] = {};
                        ImGui::InputTextWithHint("##", ConvertMemoryFlagtoString(memoryProperties.memoryProperties.memoryHeaps[i].flags).c_str(), buffer, sizeof(buffer));
                        ImGui::PopItemWidth();
                        ImGui::SameLine();
                        ImGuiH::DrawResetIcon(false);
                    });
                });
            }
        }

        ImGui::PopStyleVar();

        /**
        * @brief End render GBuffer Visualizer.
        */
        End();
    }

    std::string ImguiGPUMemoryHUD::ConvertBytestoString(uint64_t bytes)
    {
        SPICES_PROFILE_ZONE;

        if (bytes / 1024.0f < 1.0f)
        {
            std::stringstream ss;
            ss << bytes << "bytes";

            return ss.str();
        }
        else if (bytes / 1024.0f / 1024.0f < 1.0f)
        {
            std::stringstream ss;
            ss << bytes / 1024.0f << " KB";

            return ss.str();
        }
        else if (bytes / 1024.0f / 1024.0f / 1024.0f < 1.0f)
        {
            std::stringstream ss;
            ss << bytes / 1024.0f / 1024.0f << " MB";

            return ss.str();
        }
        else if (bytes / 1024.0f / 1024.0f / 1024.0f / 1024.0f < 1.0f)
        {
            std::stringstream ss;
            ss << bytes / 1024.0f / 1024.0f / 1024.0f << " GB";

            return ss.str();
        }
        else
        {
            std::stringstream ss;
            ss << bytes / 1024.0f / 1024.0f / 1024.0f / 1024.0f << " TB";

            return ss.str();
        }
    }

    std::string ImguiGPUMemoryHUD::ConvertMemoryFlagtoString(VkMemoryHeapFlags flag)
    {
        SPICES_PROFILE_ZONE;

        switch (flag)
        {
        case VK_MEMORY_HEAP_DEVICE_LOCAL_BIT:
            return "Device Local";
        case VK_MEMORY_HEAP_MULTI_INSTANCE_BIT:        // NOTICE THAT: enum value also represents "VK_MEMORY_HEAP_MULTI_INSTANCE_BIT_KHR"
            return "Multiple Instance";
        default:
            // In case that it does NOT correspond to device local memory
            return "Host Local";
        }
    }
}