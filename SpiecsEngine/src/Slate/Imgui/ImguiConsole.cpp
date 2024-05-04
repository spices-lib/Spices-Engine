#include "Pchheader.h"
#include "ImguiConsole.h"
#include "Core/Log/Console.h"
#include "Resources/ResourcePool/ResourcePool.h"
#include "Resources/Texture/Texture2D.h"
#include "Render/Vulkan/VulkanImage.h"

namespace Spiecs {

	ImguiConsole::ImguiConsole(const std::string& panelName, std::shared_ptr<Console> console)
		: ImguiSlate(panelName)
		, m_Console(console)
	{
		LoadConsoleIcon(m_ConsoleIconID.clearConsoleIcon,       "slate/Console.ClearConsole.png"       );
		LoadConsoleIcon(m_ConsoleIconID.openLogFileIcon,        "slate/Console.OpenLogFile.png"        );
		LoadConsoleIcon(m_ConsoleIconID.openLogFolderIcon,      "slate/Console.OpenLogFolder.png"      );
		LoadConsoleIcon(m_ConsoleIconID.enableCommandFieldIcon, "slate/Console.EnableCommandField.png" );

		LoadConsoleIcon(m_ConsoleIconID.select,                 "slate/Console.Select.png"             );
		LoadConsoleIcon(m_ConsoleIconID.verbose,                "slate/Console.Verbose.png"            );
		LoadConsoleIcon(m_ConsoleIconID.info,                   "slate/Console.Info.png"               );
		LoadConsoleIcon(m_ConsoleIconID.warning,                "slate/Console.Warning.png"            );
		LoadConsoleIcon(m_ConsoleIconID.error,                  "slate/Console.Error.png"              );
	}

	void ImguiConsole::OnRender()
	{
		Begin();

		if (ImGui::ImageButton(m_ConsoleIconID.clearConsoleIcon, ImVec2(18, 18))) { m_Console->Clear(); }
		ImGui::SameLine();
		if (ImGui::ImageButton(m_ConsoleIconID.openLogFileIcon, ImVec2(18, 18))) { m_Console->Clear(); }
		ImGui::SameLine();
		if (ImGui::ImageButton(m_ConsoleIconID.openLogFolderIcon, ImVec2(18, 18))) { m_Console->Clear(); }
		ImGui::SameLine();
		if (ImGui::ImageButton(m_ConsoleIconID.enableCommandFieldIcon, ImVec2(18, 18))) { m_Console->Clear(); }
		ImGui::SameLine();
		if (ImGui::ImageButton(m_ConsoleIconID.select, ImVec2(18, 18))) { m_Console->Clear(); }
		ImGui::SameLine();
		if (ImGui::ImageButton(m_ConsoleIconID.verbose, ImVec2(18, 18))) { m_Level = 0; }
		ImGui::SameLine();
		if (ImGui::ImageButton(m_ConsoleIconID.info, ImVec2(18, 18))) { m_Level = 1; }
		ImGui::SameLine();
		if (ImGui::ImageButton(m_ConsoleIconID.warning, ImVec2(18, 18))) { m_Level = 2; }
		ImGui::SameLine();
		if (ImGui::ImageButton(m_ConsoleIconID.error, ImVec2(18, 18))) { m_Level = 3; }
		ImGui::SameLine();
		char search[128] = "";
		if (ImGui::InputText("search", search, 128)) {}
		ImGui::Separator();


		const float footer_height_to_reserve = ImGui::GetStyle().ItemSpacing.y + ImGui::GetFrameHeightWithSpacing();
		ImGui::PushStyleColor(ImGuiCol_ChildBg, ImGui::GetStyleColorVec4(ImGuiCol_FrameBg));
		if (ImGui::BeginChild("ScrollingRegion", ImVec2(0, -footer_height_to_reserve), ImGuiChildFlags_None, ImGuiWindowFlags_HorizontalScrollbar))
		{
			ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4, 1));

			bool isFiltered = m_Filter.IsActive();
			//for (int i = 0; i < m_Console->GetInfos().size(); i++)
			int i = 0;
			for(auto iter = m_Console->GetInfos().rbegin(); iter != m_Console->GetInfos().rend(); iter++)
			{
				if (i == 50) break;

				const InfoLevelHelper& helper = *iter;
				ImGui::PushStyleColor(ImGuiCol_Text, { helper.color.x,  helper.color.y,  helper.color.z,  helper.color.w });

				switch (m_Level)
				{
				default:
				case 0:
					if (helper.level == "verbose")
					{
						ImGui::Text(helper.str.c_str());
						break;
					}
				case 1:
					if (helper.level == "info")
					{
						ImGui::Text(helper.str.c_str());
						break;
					}
				case 2:
					if (helper.level == "warning")
					{
						ImGui::Text(helper.str.c_str());
						break;
					}
				case 3:
					if (helper.level == "error")
					{
						ImGui::Text(helper.str.c_str());
						break;
					}
				}

				ImGui::PopStyleColor();

				i++;
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

	void ImguiConsole::LoadConsoleIcon(ImTextureID& id, const std::string& iconFile)
	{
		auto rowPtr = ResourcePool<Texture>::Load<Texture2D>(iconFile);
		auto info = rowPtr->GetResource<VulkanImage>()->GetImageInfo();

		id = ImGui_ImplVulkan_AddTexture(info->sampler, info->imageView, info->imageLayout);
	}

}