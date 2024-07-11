/**
* @file UUIDComponent.cpp.
* @brief The UUIDComponent Class Implementation.
* @author Spiecs.
*/

#include "Pchheader.h"
#include "UUIDComponent.h"

namespace Spiecs {

	void UUIDComponent::OnSerialize()
	{

	}

	void UUIDComponent::OnDeSerialize()
	{

	}

	void UUIDComponent::DrawThis()
	{
		SPIECS_PROFILE_ZONE;
		
		ImGui::Spacing();
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 3.0f });
		float columeWidth = ImGuiH::GetLineItemSize().x * 6.5f;

		{
			SPIECS_PROFILE_ZONEN("UUIDComponent UUID");
			
			ImGui::PushID("TagComponent Tags");
			ImGui::Columns(2, 0, false);
		
			ImGui::SetColumnWidth(0, columeWidth);
			ImGui::Text("UUID");
			ImGui::NextColumn();

			ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x - ImGuiH::GetLineItemSize().x);
			
			char buffer[256];
			memset(buffer, 0, sizeof(buffer));
			strcpy_s(buffer, sizeof(buffer), std::to_string(m_uuid).c_str());
			if(ImGui::InputText("##", buffer, sizeof(buffer)))
			{
				strcpy_s(buffer, sizeof(buffer), std::to_string(m_uuid).c_str());
			}
			ImGui::PopItemWidth();
			ImGui::SameLine();

			ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImGui::GetStyle().Colors[ImGuiCol_Button]);
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImGui::GetStyle().Colors[ImGuiCol_Button]);
			ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.4f, 0.4f, 0.4f, 1.0f));
			ImGui::Button(ICON_MD_CROP_SQUARE, ImGuiH::GetLineItemSize());
			ImGui::PopStyleColor(3);

			ImGui::Columns(1);
			ImGui::PopID();
		}
		
		ImGui::PopStyleVar();
		ImGui::Spacing();
	}
}
