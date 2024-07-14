/**
* @file ImguiAbout.cpp.
* @brief The ImguiAbout Class Implementation.
* @author Spices.
*/

#include "Pchheader.h"
#include "ImguiAbout.h"

#include "Systems/SlateSystem.h"

namespace Spices {

    ImguiAbout::ImguiAbout(
        const std::string& panelName , 
        FrameInfo&         frameInfo
    )
        : ImguiSlate(panelName, frameInfo)
    {}

    void ImguiAbout::OnRender()
    {
        SPICES_PROFILE_ZONE;

        if (!m_IsSlateOn) return;

        /**
        * @brief Begin render About.
        */
        Begin();

        ImGui::Text("Spices Engine Version 0.0.1");

		const float footer_height_to_reserve = ImGui::GetStyle().ItemSpacing.y + ImGui::GetFrameHeightWithSpacing();
		ImGui::PushStyleColor(ImGuiCol_ChildBg, ImGui::GetStyleColorVec4(ImGuiCol_FrameBg));
		if (ImGui::BeginChild("ScrollingRegion", ImVec2(0, -footer_height_to_reserve), ImGuiChildFlags_None, ImGuiWindowFlags_HorizontalScrollbar))
		{
			ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4, 1));

            ImGui::Text("Spices Engine Version 0.0.1\n\n");


			ImGui::PopStyleVar();
		}
		ImGui::PopStyleColor();
		ImGui::EndChild();

        /**
        * @brief End render About.
        */
        End();
    }
}