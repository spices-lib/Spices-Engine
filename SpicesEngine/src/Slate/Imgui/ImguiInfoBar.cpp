/**
* @file ImguiOperation.cpp.
* @brief The ImguiOperation Class Implementation.
* @author Spices.
*/

#include "Pchheader.h"
#include "ImguiInfoBar.h"
#include "World/World/World.h"
#include "Slate/SlateInfoBar.h"

namespace Spices {

    void ImguiInfoBar::OnUpdate(TimeStep& ts)
    {
        SPICES_PROFILE_ZONE;

        /**
        * @brief Clear slected infobar.
        */
        if (m_SelectedInfoBar)
        {
            if (m_SelectedInfoBar->IsDestroy())
            {
                m_SelectedInfoBar = nullptr;
            }
        }

        std::vector<std::string> eraseList;

        m_InfoBars.for_each([&](auto& k, auto& v) {

            if (v->IsDestroy())
            {
                eraseList.push_back(v->GetInfo());
            }

            return false;
        });

        /**
        * @brief Erase completed InfoBar.
        */
        for (auto& item : eraseList)
        {
            m_InfoBars.erase(item);
        }
    }

    void ImguiInfoBar::OnRender()
    {
        SPICES_PROFILE_ZONE;
        
        /**
        * @brief Begin render ContentBrowser.
        */
        Begin(m_PanelName, 1.0f, ImGuiWindowFlags_NoTitleBar);

        {
            SPICES_PROFILE_ZONEN("ImguiInfoBar");

            float width = ImGui::GetContentRegionAvail().x;
            ImGui::Spacing();
            
            /**
            * @brief Skip if no InfoBar Instance.
            */
            if (m_InfoBars.size() == 0)
            {
                End();
                return;
            }

            ImGui::PushID("ImguiInfoBar");

            auto item = m_SelectedInfoBar ? m_SelectedInfoBar : *m_InfoBars.first();

            ImGui::Columns(3, 0);

            ImGui::SetColumnWidth(0, 0.5f * width);
            ImGui::NextColumn();
                
            ImGui::SetColumnWidth(1, 0.39f * width);
            ImGui::Text(item->GetInfo().c_str());
            ImGui::NextColumn();
                
            switch (item->GetType())
            {
            case SlateInfoBar::Type::progress:
            {
                float rate = std::any_cast<float>(item->GetRate());
                ImGui::PushStyleColor(ImGuiCol_PlotHistogram, ImVec4(0.310f, 0.49f, 0.627f, 1.0f));
                ImGui::ProgressBar(rate);
                ImGui::PopStyleColor();
                break;
            }
            case SlateInfoBar::Type::count:
            {
                int count = std::any_cast<int>(item->GetRate());
                std::stringstream ss;
                ss << count;

                ImGui::Text(ss.str().c_str());
                break;
            }
            }

            ImGui::Columns(1);

            ImGui::PopID();
        }

        {
            SPICES_PROFILE_ZONEN("ImguiInfoBar::DisplayList");

            if (ImGui::BeginPopupContextWindow(0, 1))
            {
                m_InfoBars.for_each([&](auto& k, auto& v) {

                    if (ImGui::MenuItem(k.c_str()))
                    {
                        m_SelectedInfoBar = v;
                    }
                    return false;

                });

                ImGui::EndPopup();
            }
        }

        /**
        * @brief End render ContentBrowser.
        */
        End();
    }

    void ImguiInfoBar::Push(std::shared_ptr<SlateInfoBar> instance)
    {
        SPICES_PROFILE_ZONE;

        m_InfoBars.push_back(instance->GetInfo(), instance);
    }
}
