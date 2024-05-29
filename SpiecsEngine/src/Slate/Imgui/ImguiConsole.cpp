/**
* @file ImguiGizmos.cpp.
* @brief The ImguiGizmos Class Implementation.
* @author Spiecs.
*/

#include "Pchheader.h"
#include "ImguiConsole.h"

#include "Core/Log/Console.h"
#include "Resources/ResourcePool/ResourcePool.h"
#include "Resources/Texture/Texture2D.h"
#include "Core/Library/FileLibrary.h"
#include "Render/Vulkan/VulkanRenderBackend.h"
#include "Core/Library/ProcessLibrary.h"

namespace Spiecs {

	ImguiConsole::ImguiConsole(
		const std::string&       panelName , 
		FrameInfo&               frameInfo , 
		std::shared_ptr<Console> console
	)
		: ImguiSlate(panelName, frameInfo)
		, m_Console(console)
	{
		ZoneScoped;

		/**
		* @breif Load Icons that need.
		*/
		LoadSlateIcon(m_ConsoleIconID.clearConsoleIcon,       "slate/console/Console.ClearConsole.png"       );
		LoadSlateIcon(m_ConsoleIconID.openLogFileIcon,        "slate/console/Console.OpenLogFile.png"        );
		LoadSlateIcon(m_ConsoleIconID.openLogFolderIcon,      "slate/console/Console.OpenLogFolder.png"      );
		LoadSlateIcon(m_ConsoleIconID.enableCommandFieldIcon, "slate/console/Console.EnableCommandField.png" );

		LoadSlateIcon(m_ConsoleIconID.select,                 "slate/console/Console.Select.png"             );
		LoadSlateIcon(m_ConsoleIconID.verbose,                "slate/console/Console.Verbose.png"            );
		LoadSlateIcon(m_ConsoleIconID.info,                   "slate/console/Console.Info.png"               );
		LoadSlateIcon(m_ConsoleIconID.warning,                "slate/console/Console.Warning.png"            );
		LoadSlateIcon(m_ConsoleIconID.error,                  "slate/console/Console.Error.png"              );
	}

	void ImguiConsole::OnRender()
	{
		ZoneScoped;

		/**
		* @brief Begin render Console.
		*/
		Begin();

		/**
		* @brief Render ClearConsoleIcon.
		*/
		{
			ZoneScopedN("Render ClearConsoleIcon");

			if (ImGui::ImageButton(m_ConsoleIconID.clearConsoleIcon, ImVec2(18, 18)))
			{
				m_Console->Clear();
			}

			ImGui::SameLine();
		}

		/**
		* @brief Render OpenLogFileIcon.
		*/
		{
			ZoneScopedN("Render OpenLogFileIcon");

			if (ImGui::ImageButton(m_ConsoleIconID.openLogFileIcon, ImVec2(18, 18)))
			{
				std::stringstream ss;
				ss << "C:/Windows/System32/notepad.exe " << m_Console->GetFilePath();

				/**
				* @brief Open Log file that in writtening.
				*/
				ProcessLibrary::OpenProcess("C:/Windows/System32/notepad.exe ", ss.str().c_str());
			}

			ImGui::SameLine();
		}

		/**
		* @brief Render OpenLogFolderIcon.
		*/
		{
			ZoneScopedN("Render OpenLogFolderIcon");

			if (ImGui::ImageButton(m_ConsoleIconID.openLogFolderIcon, ImVec2(18, 18)))
			{
				/**
				* @brief Get Selected file in Explore.
				*/
				std::string filepath = FileLibrary::FileLibrary_OpenInExplore(
					"Spiecs Log (*.log)\0*.log\0",
					glfwGetWin32Window((GLFWwindow*)VulkanRenderBackend::GetState().m_Windows)
				);

				if (filepath != "")
				{
					std::stringstream ss;
					ss << "C:/Windows/System32/notepad.exe " << filepath;

					/**
					* @brief Open Log file that in writtening.
					*/
					ProcessLibrary::OpenProcess("C:/Windows/System32/notepad.exe ", ss.str().c_str());
				}
			}

			ImGui::SameLine();
		}

		/**
		* @brief Render EnableCommandFieldIcon.
		*/
		{
			ZoneScopedN("Render EnableCommandFieldIcon");

			if (ImGui::ImageButton(m_ConsoleIconID.enableCommandFieldIcon, ImVec2(18, 18)))
			{
				m_Console->Clear();
			}

			ImGui::SameLine();
		}

		/**
		* @brief Render SelectIcon.
		*/
		{
			ZoneScopedN("Render SelectIcon");

			if (ImGui::ImageButton(m_ConsoleIconID.select, ImVec2(18, 18)))
			{
				m_Console->Clear();
			}

			ImGui::SameLine();
		}

		/**
		* @brief Render VerboseIcon.
		*/
		{
			ZoneScopedN("Render VerboseIcon");

			if (ImGui::ImageButton(m_ConsoleIconID.verbose, ImVec2(18, 18)))
			{
				m_Level = 0;
			}

			ImGui::SameLine();
		}

		/**
		* @brief Render InfoIcon.
		*/
		{
			ZoneScopedN("Render InfoIcon");

			if (ImGui::ImageButton(m_ConsoleIconID.info, ImVec2(18, 18)))
			{
				m_Level = 1;
			}

			ImGui::SameLine();
		}

		/**
		* @brief Render WarningIcon.
		*/
		{
			ZoneScopedN("Render WarningIcon");

			if (ImGui::ImageButton(m_ConsoleIconID.warning, ImVec2(18, 18))) 
			{ 
				m_Level = 2; 
			}

			ImGui::SameLine();
		}

		/**
		* @brief Render ErrorIcon.
		*/
		{
			ZoneScopedN("Render ErrorIcon");

			if (ImGui::ImageButton(m_ConsoleIconID.error, ImVec2(18, 18))) 
			{
				m_Level = 3; 
			}

			ImGui::SameLine();
		}

		/**
		* @brief Render Search Input Text.
		*/
		{
			ZoneScopedN("Render Search Input Text");

			char search[128] = "";
			if (ImGui::InputText("Search", search, 128)) {}

			ImGui::Separator();
		}

		/**
		* @brief Render Console ScrollingRegion.
		*/
		{
			ZoneScopedN("Render Console ScrollingRegion");

			const float footer_height_to_reserve = ImGui::GetStyle().ItemSpacing.y + ImGui::GetFrameHeightWithSpacing();
			ImGui::PushStyleColor(ImGuiCol_ChildBg, ImGui::GetStyleColorVec4(ImGuiCol_FrameBg));
			ImGui::PushStyleColor(ImGuiCol_HeaderHovered, ImGui::GetStyleColorVec4(ImGuiCol_Border));

			if (ImGui::BeginChild("ScrollingRegion", ImVec2(0, -footer_height_to_reserve), ImGuiChildFlags_None, ImGuiWindowFlags_HorizontalScrollbar))
			{
				ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4, 1));

				bool isFiltered = m_Filter.IsActive();

				int i = 0;
				for (auto iter = m_Console->GetInfos().rbegin(); iter != m_Console->GetInfos().rend(); iter++)
				{
					if (i == 50) break;

					const InfoLevelHelper& helper = *iter;
					ImGui::PushStyleColor(ImGuiCol_Text, { helper.color.x,  helper.color.y,  helper.color.z,  helper.color.w });

					switch (m_Level)
					{
					case 0:
						if (helper.level == "trace")
						{
							ImGui::Selectable(helper.str.c_str(), false);
						}
						break;
					case 1:
						if (helper.level == "info")
						{
							ImGui::Selectable(helper.str.c_str(), false);
						}
						break;
					case 2:
						if (helper.level == "warn")
						{
							ImGui::Selectable(helper.str.c_str(), false);
						}
						break;
					case 3:
						if (helper.level == "error")
						{
							ImGui::Selectable(helper.str.c_str(), false);
						}
						break;
					}

					ImGui::PopStyleColor();

					i++;
				}

				ImGui::PopStyleVar();
			}

			ImGui::PopStyleColor(2);
			ImGui::EndChild();
			ImGui::Separator();
		}

		/**
		* @brief Render Console Command-line.
		*/
		{
			ZoneScopedN("Render Console Command-line");

			bool reclaim_focus = false;
			char InputBuf[512] = "";
			ImGuiInputTextFlags input_text_flags = ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_EscapeClearsAll | ImGuiInputTextFlags_CallbackCompletion | ImGuiInputTextFlags_CallbackHistory;
			if (ImGui::InputText("cmd", InputBuf, IM_ARRAYSIZE(InputBuf), input_text_flags, &ImguiConsole::TextEditCallbackStub, (void*)this))
			{
			}
		}

		/**
		* @brief End render Console.
		*/
		End();
	}

	int ImguiConsole::TextEditCallbackStub(ImGuiInputTextCallbackData* data)
	{
		return 0;
	}
}