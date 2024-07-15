/**
* @file ImguiConsole.cpp
* @brief The ImguiConsole Class Implementation.
* @author Spices.
*/

#include "Pchheader.h"
#include "ImguiConsole.h"

#include "Core/Log/Console.h"
#include "Core/Library/FileLibrary.h"
#include "Render/Vulkan/VulkanRenderBackend.h"
#include "Core/Library/ProcessLibrary.h"

namespace Spices {

	ImguiConsole::ImguiConsole(
		const std::string&       panelName , 
		FrameInfo&               frameInfo , 
		std::shared_ptr<Console> console
	)
		: ImguiSlate(panelName, frameInfo)
		, m_Console(console)
	{}

	void ImguiConsole::OnRender()
	{
		SPICES_PROFILE_ZONE;

		/**
		* @brief Begin render Console.
		*/
		Begin();

		ImGui::Spacing();

		/**
		* @brief Render ClearConsoleIcon.
		*/
		{
			SPICES_PROFILE_ZONEN("Console ClearConsoleIcon");

			if (ImGui::Button(ICON_MD_FORMAT_CLEAR, ImGuiH::GetLineItemSize()))
			{
				m_Console->Clear();
			}
			ImGui::SetItemTooltip("Clear Console");
		}

		/**
		* @brief Render OpenLogFileIcon.
		*/
		{
			SPICES_PROFILE_ZONEN("Console OpenLogFileIcon");

			ImGui::SameLine();
			if (ImGui::Button(ICON_MD_EDIT_NOTE, ImGuiH::GetLineItemSize()))
			{
				std::stringstream ss;
				ss << "C:/Windows/System32/notepad.exe " << m_Console->GetFilePath();

				/**
				* @brief Open Log file that in writtening.
				*/
				ProcessLibrary::OpenProcess("C:/Windows/System32/notepad.exe ", ss.str().c_str());
			}
			ImGui::SetItemTooltip("Open Log File");
		}

		/**
		* @brief Render OpenLogFolderIcon.
		*/
		{
			SPICES_PROFILE_ZONEN("Console OpenLogFolderIcon");

			ImGui::SameLine();
			if (ImGui::Button(ICON_MD_FOLDER, ImGuiH::GetLineItemSize()))
			{
				/**
				* @brief Get Selected file in Explore.
				*/
				std::string filepath = FileLibrary::FileLibrary_OpenInExplore(
					"Spices Log (*.log)\0*.log\0",
					glfwGetWin32Window((GLFWwindow*)VulkanRenderBackend::GetState().m_Windows)
				);

				if (!filepath.empty())
				{
					std::stringstream ss;
					ss << "C:/Windows/System32/notepad.exe " << filepath;

					/**
					* @brief Open Log file that in writtening.
					*/
					ProcessLibrary::OpenProcess("C:/Windows/System32/notepad.exe ", ss.str().c_str());
				}
			}
			ImGui::SetItemTooltip("Open Log Folder");
		}

		/**
		* @brief Render EnableCommandFieldIcon.
		*/
		{
			SPICES_PROFILE_ZONEN("Console EnableCommandFieldIcon");

			ImGui::SameLine();
			if (ImGui::Button(ICON_MD_TERMINAL, ImGuiH::GetLineItemSize())) m_EnableCmdInput = !m_EnableCmdInput;
			ImGui::SetItemTooltip("Enable Command Field");
		}

		/**
		* @brief Render FilterIcon.
		*/
		{
			SPICES_PROFILE_ZONEN("Console FilterIcon");

			ImGui::SameLine(m_PanelSize.x - ImGuiH::GetLineItemSize().x * 5.7f - 220.0f);
			if (ImGui::Button(ICON_MD_FILTER_ALT, ImGuiH::GetLineItemSize())) {}
			ImGui::SetItemTooltip("Filter");
		}

		/**
		* @brief Render VerboseIcon.
		*/
		{
			SPICES_PROFILE_ZONEN("Console VerboseIcon");

			ImGui::SameLine();
			ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.83f, 0.83f, 0.83f, 1.0f));
			if (ImGui::Button(ICON_MD_EMERGENCY, ImGuiH::GetLineItemSize())) m_Level = 0;
			ImGui::PopStyleColor();
			ImGui::SetItemTooltip("Verbose");
		}

		/**
		* @brief Render InfoIcon.
		*/
		{
			SPICES_PROFILE_ZONEN("Console InfoIcon");

			ImGui::SameLine();
			ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.574f, 0.829f, 1.0f, 1.0f));
			if (ImGui::Button(ICON_MD_ERROR_OUTLINE, ImGuiH::GetLineItemSize())) m_Level = 1;
			ImGui::PopStyleColor();
			ImGui::SetItemTooltip("Info");
		}

		/**
		* @brief Render WarningIcon.
		*/
		{
			SPICES_PROFILE_ZONEN("Console WarningIcon");

			ImGui::SameLine();
			ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.974f, 0.896f, 0.39f, 1.0f));
			if (ImGui::Button(ICON_MD_WARNING_AMBER, ImGuiH::GetLineItemSize())) m_Level = 2;
			ImGui::PopStyleColor();
			ImGui::SetItemTooltip("Warning");
			ImGui::SameLine();
			ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.974f, 0.896f, 0.39f, 1.0f));
			ImGui::Text(std::to_string(m_Console->GetInfos().m_WarnLogInfos.size()).c_str());
			ImGui::PopStyleColor();
		}

		/**
		* @brief Render ErrorIcon.
		*/
		{
			SPICES_PROFILE_ZONEN("Console ErrorIcon");

			ImGui::SameLine();
			ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.641f, 0.59f, 1.0f));
			if (ImGui::Button(ICON_MD_NEARBY_ERROR, ImGuiH::GetLineItemSize())) m_Level = 3;
			ImGui::PopStyleColor();
			ImGui::SetItemTooltip("Error");
			ImGui::SameLine();
			ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.641f, 0.59f, 1.0f));
			ImGui::Text(std::to_string(m_Console->GetInfos().m_ErrorLogInfos.size()).c_str());
			ImGui::PopStyleColor();
		}

		/**
		* @brief Search String.
		*/
		static std::string searchString;
		static bool isEnableSearch = false;

		/**
		* @brief Render Search Input Text.
		*/
		{
			SPICES_PROFILE_ZONEN("Console Search Input Text");

			ImGui::SameLine();
			ImGui::PushItemWidth(200);
			static char search[128] = "";
			if (ImGui::InputTextWithHint("##", ICON_TEXT(ICON_MD_SEARCH, Search), search, 128)) 
			{
				searchString = std::string(search);
				if (searchString.size() == 0) isEnableSearch = false;
				else isEnableSearch = true;
			}
			ImGui::PopItemWidth();
			ImGui::Spacing();
			ImGui::PushStyleColor(ImGuiCol_Separator, ImVec4(0.121f, 0.129f, 0.141f, 1.0f));
			ImGui::Separator();
			ImGui::PopStyleColor();
			ImGui::Spacing();
		}

		/**
		* @brief Render Console ScrollingRegion.
		*/
		{
			SPICES_PROFILE_ZONEN("Console ScrollingRegion");

			ImGui::PushStyleColor(ImGuiCol_ChildBg, ImGui::GetStyleColorVec4(ImGuiCol_FrameBg));
			ImGui::PushStyleColor(ImGuiCol_HeaderHovered, ImGui::GetStyleColorVec4(ImGuiCol_Border));

			float itemHeight = m_EnableCmdInput ? 3.0f * ImGui::GetStyle().ItemSpacing.y + ImGui::GetFrameHeightWithSpacing() : ImGui::GetStyle().ItemSpacing.y;
			if (ImGui::BeginChild("ScrollingRegion", ImVec2(0, -itemHeight), ImGuiChildFlags_None, ImGuiWindowFlags_HorizontalScrollbar))
			{
				ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4, 1));

				bool isFiltered = m_Filter.IsActive();

				switch (m_Level)
				{
				default:
					break;
				case 0:
					for (auto it = m_Console->GetInfos().m_TraceLogInfos.rbegin(); it != m_Console->GetInfos().m_TraceLogInfos.rend(); ++it)
					{
						const InfoLevelHelper& helper = *it;

						if (isEnableSearch)
						{
							if (helper.str.find(searchString) == std::string::npos) continue;
						}

						ImGui::PushStyleColor(ImGuiCol_Text, { helper.color.x,  helper.color.y,  helper.color.z,  helper.color.w });
						if (helper.level == "trace")
						{
							std::stringstream ss;
							ss << ICON_MD_EMERGENCY << " " << helper.str;

							ImGui::Selectable(ss.str().c_str(), false);
						}
						ImGui::PopStyleColor();
					}
					break;
				case 1:
					for (auto it = m_Console->GetInfos().m_InfoLogInfos.rbegin(); it != m_Console->GetInfos().m_InfoLogInfos.rend(); ++it)
					{
						const InfoLevelHelper& helper = *it;

						if (isEnableSearch)
						{
							if (helper.str.find(searchString) == std::string::npos) continue;
						}

						ImGui::PushStyleColor(ImGuiCol_Text, { helper.color.x,  helper.color.y,  helper.color.z,  helper.color.w });
						if (helper.level == "info")
						{
							std::stringstream ss;
							ss << ICON_MD_ERROR_OUTLINE << " " << helper.str;

							ImGui::Selectable(ss.str().c_str(), false);
						}
						ImGui::PopStyleColor();
					}
					break;
				case 2:
					for (auto it = m_Console->GetInfos().m_WarnLogInfos.rbegin(); it != m_Console->GetInfos().m_WarnLogInfos.rend(); ++it)
					{
						const InfoLevelHelper& helper = *it;

						if (isEnableSearch)
						{
							if (helper.str.find(searchString) == std::string::npos) continue;
						}

						ImGui::PushStyleColor(ImGuiCol_Text, { helper.color.x,  helper.color.y,  helper.color.z,  helper.color.w });
						if (helper.level == "warn")
						{
							std::stringstream ss;
							ss << ICON_MD_WARNING_AMBER << " " << helper.str;

							ImGui::Selectable(ss.str().c_str(), false);
						}
						ImGui::PopStyleColor();
					}
					break;
				case 3:
					for (auto it = m_Console->GetInfos().m_ErrorLogInfos.rbegin(); it != m_Console->GetInfos().m_ErrorLogInfos.rend(); ++it)
					{
						const InfoLevelHelper& helper = *it;

						if (isEnableSearch)
						{
							if (helper.str.find(searchString) == std::string::npos) continue;
						}

						ImGui::PushStyleColor(ImGuiCol_Text, { helper.color.x,  helper.color.y,  helper.color.z,  helper.color.w });
						if (helper.level == "error")
						{
							std::stringstream ss;
							ss << ICON_MD_NEARBY_ERROR << " " << helper.str;

							ImGui::Selectable(ss.str().c_str(), false);
						}
						ImGui::PopStyleColor();
					}
					break;
				}

				ImGui::PopStyleVar();
			}

			ImGui::PopStyleColor(2);
			ImGui::EndChild();
		}

		/**
		* @brief Render Console Command-line.
		*/
		{
			if (m_EnableCmdInput)
			{
				SPICES_PROFILE_ZONEN("Console Command-line");

				ImGui::Spacing();
				ImGui::Separator();
				ImGui::Spacing();
				ImGui::PushItemWidth(-1);
				bool reclaim_focus = false;
				char InputBuf[512] = "";
				ImGuiInputTextFlags input_text_flags = 
					ImGuiInputTextFlags_EnterReturnsTrue   | 
					ImGuiInputTextFlags_EscapeClearsAll    | 
					ImGuiInputTextFlags_CallbackCompletion | 
					ImGuiInputTextFlags_CallbackHistory    |
					ImGuiInputTextFlags_CallbackCharFilter ;
				ImGui::InputTextWithHint(
					"Cmd", 
					"Cmd", 
					InputBuf, 
					IM_ARRAYSIZE(InputBuf), 
					input_text_flags, 
					&ImguiConsole::TextEditCallbackStub, 
					(void*)this
				);
				ImGui::PopItemWidth();
			}
		}

		/**
		* @brief End render Console.
		*/
		End();
	}

	int ImguiConsole::TextEditCallbackStub(ImGuiInputTextCallbackData* data)
	{
		SPICES_CORE_INFO(data->Buf);

		return 0;
	}
}