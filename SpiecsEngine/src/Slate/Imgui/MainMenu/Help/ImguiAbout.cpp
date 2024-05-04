#include "Pchheader.h"
#include "Systems/SlateSystem.h"
#include "ImguiAbout.h"

namespace Spiecs {

    ImguiAbout::ImguiAbout(const std::string& panelName)
        : ImguiSlate(panelName)
    {
    }

    void ImguiAbout::OnRender()
    {
        if (!m_IsSlateOn) return;

        Begin();

        ImGui::Text("Spiecs Engine Version 0.0.1");

		const float footer_height_to_reserve = ImGui::GetStyle().ItemSpacing.y + ImGui::GetFrameHeightWithSpacing();
		ImGui::PushStyleColor(ImGuiCol_ChildBg, ImGui::GetStyleColorVec4(ImGuiCol_FrameBg));
		if (ImGui::BeginChild("ScrollingRegion", ImVec2(0, -footer_height_to_reserve), ImGuiChildFlags_None, ImGuiWindowFlags_HorizontalScrollbar))
		{
			ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4, 1));

            ImGui::Text("Spiecs Engine Version 0.0.1\n\n");


			ImGui::PopStyleVar();
		}
		ImGui::PopStyleColor();
		ImGui::EndChild();


        End();
    }
}