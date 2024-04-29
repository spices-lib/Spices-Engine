#include "Pchheader.h"
#include "ImguiConsole.h"
#include "Core/Log/Console.h"

namespace Spiecs {

	ImguiConsole::ImguiConsole(const std::string& panelName, std::shared_ptr<Console> console)
		: ImguiSlate(panelName)
		, m_Console(console)
	{

	}

	void ImguiConsole::OnRender()
	{
		Begin();

		if (ImGui::SmallButton("Clear")) { m_Console->Clear(); }
		ImGui::Separator();

		m_Filter.Draw("Filter", -100.0f);

		const float footer_height_to_reserve = ImGui::GetStyle().ItemSpacing.y + ImGui::GetFrameHeightWithSpacing();
		ImGui::PushStyleColor(ImGuiCol_ChildBg, ImGui::GetStyleColorVec4(ImGuiCol_FrameBg));
		if (ImGui::BeginChild("ScrollingRegion", ImVec2(0, -footer_height_to_reserve), ImGuiChildFlags_None, ImGuiWindowFlags_HorizontalScrollbar))
		{
			ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4, 1));

			bool isFiltered = m_Filter.IsActive();
			for (int i = 0; i < m_Console->GetInfos().size(); i++)
			{
				const InfoLevelHelper& helper = m_Console->GetInfos()[i];
				ImGui::PushStyleColor(ImGuiCol_Text, { helper.color.x,  helper.color.y,  helper.color.z,  helper.color.w });
				if (isFiltered)
				{
					if (m_Filter.PassFilter(helper.str.c_str()))
					{
						ImGui::TextUnformatted(helper.str.c_str());
					}
				}
				else
				{		
					ImGui::TextUnformatted(helper.str.c_str());
				}
				ImGui::PopStyleColor();
			}

			ImGui::PopStyleVar();
		}
		ImGui::PopStyleColor();
		ImGui::EndChild();
		ImGui::Separator();

		// Command-line
		bool reclaim_focus = false;
		char InputBuf[512] = "";
		ImGuiInputTextFlags input_text_flags = ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_EscapeClearsAll | ImGuiInputTextFlags_CallbackCompletion | ImGuiInputTextFlags_CallbackHistory;
		if (ImGui::InputText("cmd", InputBuf, IM_ARRAYSIZE(InputBuf), input_text_flags, &ImguiConsole::TextEditCallbackStub, (void*)this))
		{
		}

		End();
	}

	int ImguiConsole::TextEditCallbackStub(ImGuiInputTextCallbackData* data)
	{
		return 0;
	}

}