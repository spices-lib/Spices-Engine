/**
* @file ImguiOperation.cpp.
* @brief The ImguiOperation Class Implementation.
* @author Spices.
*/

#include "Pchheader.h"
#include "ImguiInfoBar.h"

#include "World/World/World.h"

namespace Spices {

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
            
            ImGui::PushID("ImguiInfoBar");
            ImGui::Columns(3, 0);

            ImGui::SetColumnWidth(0, 0.5f * width);
            ImGui::NextColumn();

            

            float rate = 1.0f;
            auto view = FrameInfo::Get().m_World->GetRegistry().view<CameraComponent>();
            for(auto& e : view)
            {
                auto& camComp = FrameInfo::Get().m_World->GetRegistry().get<CameraComponent>(e);

                if(camComp.IsActive())
                {
                    rate = static_cast<float>(camComp.GetCamera()->GetStableFrames()) / 100.0f;
                    break;
                }
            }
            
            if(rate <= 1.0f)
            {
                ImGui::SetColumnWidth(1, 0.39f * width);
                ImGui::Text("Refrush frame");
                ImGui::NextColumn();

                ImGui::PushStyleColor(ImGuiCol_PlotHistogram, ImVec4(0.310f, 0.49f, 0.627f, 1.0f));
                ImGui::ProgressBar(rate);
                ImGui::PopStyleColor();
            }
            else
            {
                ImGui::SetColumnWidth(1, 0.39f * width);
                ImGui::NextColumn();
            }
            ImGui::Columns(1);
            ImGui::PopID();
        }

       
        
        /**
        * @brief End render ContentBrowser.
        */
        End();
    }
}
