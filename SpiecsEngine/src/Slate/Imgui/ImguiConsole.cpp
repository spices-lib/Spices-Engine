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
	{}

	void ImguiConsole::OnRender()
	{
		SPIECS_PROFILE_ZONE;

		/**
		* @brief Begin render Console.
		*/
		Begin();

		ImGui::Separator();

		/**
		* @brief Render ClearConsoleIcon.
		*/
		{
			SPIECS_PROFILE_ZONEN("Render ClearConsoleIcon");

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
			SPIECS_PROFILE_ZONEN("Render OpenLogFileIcon");

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
			SPIECS_PROFILE_ZONEN("Render OpenLogFolderIcon");

			ImGui::SameLine();
			if (ImGui::Button(ICON_MD_FOLDER, ImGuiH::GetLineItemSize()))
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
			ImGui::SetItemTooltip("Open Log Folder");
		}

		/**
		* @brief Render EnableCommandFieldIcon.
		*/
		{
			SPIECS_PROFILE_ZONEN("Render EnableCommandFieldIcon");

			ImGui::SameLine();
			if (ImGui::Button(ICON_MD_TERMINAL, ImGuiH::GetLineItemSize())) m_EnableCmdInput = !m_EnableCmdInput;
			ImGui::SetItemTooltip("Enable Command Field");
		}

		/**
		* @brief Render FilterIcon.
		*/
		{
			SPIECS_PROFILE_ZONEN("Render FilterIcon");

			ImGui::SameLine(m_PanelSize.x - ImGuiH::GetLineItemSize().x * 5.7 - 220.0f);
			if (ImGui::Button(ICON_MD_FILTER_LIST, ImGuiH::GetLineItemSize())) {}
			ImGui::SetItemTooltip("Filter");
		}

		/**
		* @brief Render VerboseIcon.
		*/
		{
			SPIECS_PROFILE_ZONEN("Render VerboseIcon");

			ImGui::SameLine();
			if (ImGui::Button(ICON_MD_RUNNING_WITH_ERRORS, ImGuiH::GetLineItemSize())) m_Level = 0;
			ImGui::SetItemTooltip("Verbose");
		}

		/**
		* @brief Render InfoIcon.
		*/
		{
			SPIECS_PROFILE_ZONEN("Render InfoIcon");

			ImGui::SameLine();
			ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.0f, 1.0f, 0.0f, 1.0f));
			if (ImGui::Button(ICON_MD_ERROR_OUTLINE, ImGuiH::GetLineItemSize())) m_Level = 1;
			ImGui::SetItemTooltip("Info");
			ImGui::PopStyleColor();
		}

		/**
		* @brief Render WarningIcon.
		*/
		{
			SPIECS_PROFILE_ZONEN("Render WarningIcon");

			ImGui::SameLine();
			ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 0.0f, 1.0f));
			if (ImGui::Button(ICON_MD_WARNING_AMBER, ImGuiH::GetLineItemSize())) m_Level = 2;
			ImGui::SetItemTooltip("Warning");
			ImGui::SameLine();
			std::string num = std::to_string(m_Console->GetInfos().m_WarnLogInfos.size());
			ImGui::Text(num.c_str());
			ImGui::PopStyleColor();
		}

		/**
		* @brief Render ErrorIcon.
		*/
		{
			SPIECS_PROFILE_ZONEN("Render ErrorIcon");

			ImGui::SameLine();
			ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.0f, 0.0f, 1.0f));
			if (ImGui::Button(ICON_MD_NEARBY_ERROR, ImGuiH::GetLineItemSize())) m_Level = 3;
			ImGui::SetItemTooltip("Error");
			ImGui::SameLine();
			std::string num = std::to_string(m_Console->GetInfos().m_ErrorLogInfos.size());
			ImGui::Text(num.c_str());
			ImGui::PopStyleColor();
		}

		/**
		* @brief Render Search Input Text.
		*/
		{
			SPIECS_PROFILE_ZONEN("Render Search Input Text");

			ImGui::SameLine();
			ImGui::PushItemWidth(200);
			char search[128] = "";
			if (ImGui::InputTextWithHint("##", "Search", search, 128)) {}
			ImGui::PopItemWidth();
			ImGui::Separator();
		}

		/**
		* @brief Render Console ScrollingRegion.
		*/
		{
			SPIECS_PROFILE_ZONEN("Render Console ScrollingRegion");

			ImGui::PushStyleColor(ImGuiCol_ChildBg, ImGui::GetStyleColorVec4(ImGuiCol_FrameBg));
			ImGui::PushStyleColor(ImGuiCol_HeaderHovered, ImGui::GetStyleColorVec4(ImGuiCol_Border));

			float itemHeight = m_EnableCmdInput ? ImGui::GetStyle().ItemSpacing.y + ImGui::GetFrameHeightWithSpacing() : ImGui::GetStyle().ItemSpacing.y;
			if (ImGui::BeginChild("ScrollingRegion", ImVec2(0, -itemHeight), ImGuiChildFlags_None, ImGuiWindowFlags_HorizontalScrollbar))
			{
				ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4, 1));

				bool isFiltered = m_Filter.IsActive();

				switch (m_Level)
				{
				case 0:
					for (auto it = m_Console->GetInfos().m_TraceLogInfos.rbegin(); it != m_Console->GetInfos().m_TraceLogInfos.rend(); it++)
					{
						const InfoLevelHelper& helper = *it;
						ImGui::PushStyleColor(ImGuiCol_Text, { helper.color.x,  helper.color.y,  helper.color.z,  helper.color.w });
						if (helper.level == "trace")
						{
							std::stringstream ss;
							ss << ICON_MD_RUNNING_WITH_ERRORS << " " << helper.str;

							ImGui::Selectable(ss.str().c_str(), false);
						}
						ImGui::PopStyleColor();
					}
					break;
				case 1:
					for (auto it = m_Console->GetInfos().m_InfoLogInfos.rbegin(); it != m_Console->GetInfos().m_InfoLogInfos.rend(); it++)
					{
						const InfoLevelHelper& helper = *it;
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
					for (auto it = m_Console->GetInfos().m_WarnLogInfos.rbegin(); it != m_Console->GetInfos().m_WarnLogInfos.rend(); it++)
					{
						const InfoLevelHelper& helper = *it;
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
					for (auto it = m_Console->GetInfos().m_ErrorLogInfos.rbegin(); it != m_Console->GetInfos().m_ErrorLogInfos.rend(); it++)
					{
						const InfoLevelHelper& helper = *it;
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
				SPIECS_PROFILE_ZONEN("Render Console Command-line");

				ImGui::Separator();
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
		SPIECS_CORE_INFO(data->Buf);

		return 0;
	}
}