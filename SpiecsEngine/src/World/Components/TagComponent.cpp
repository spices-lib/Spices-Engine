/**
* @file TagComponent.cpp.
* @brief The TagComponent Class Implementation.
* @author Spiecs.
*/

#include "Pchheader.h"
#include "TagComponent.h"

namespace Spiecs {

	TagComponent::TagComponent(const std::string& tag)
	{
		m_Tags.insert(tag);
	}

	void TagComponent::OnSerialize()
	{

	}

	void TagComponent::OnDeSerialize()
	{

	}

	void TagComponent::DrawThis()
	{
		SPIECS_PROFILE_ZONE;

		std::vector<std::string> tagBuffer(m_Tags.begin(), m_Tags.end());
		
		ImGui::Spacing();
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 3.0f });
		float columeWidth = ImGuiH::GetLineItemSize().x * 6.5f;

		{
			SPIECS_PROFILE_ZONEN("TagComponent Tags");
			
			for(int i = 0; i < tagBuffer.size(); i++)
			{
				ImGui::PushID("TagComponent Tags");
				ImGui::Columns(2, 0, false);
			
				ImGui::SetColumnWidth(0, columeWidth);
				std::stringstream ss;
				ss << "Tag[" << i << "]";
				ImGui::Text(ss.str().c_str());
				ImGui::NextColumn();
			
				ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x - ImGuiH::GetLineItemSize().x);

				char buffer[256];
				memset(buffer, 0, sizeof(buffer));
				strcpy_s(buffer, sizeof(buffer), tagBuffer[i].c_str());
				if(ImGui::InputText("##", buffer, sizeof(buffer)))
				{
					m_Tags.erase(tagBuffer[i]);
					m_Tags.insert(std::string(buffer));
				}
				ImGui::PopItemWidth();
				ImGui::SameLine();
				ImGuiH::DrawResetIcon(false);
				ImGui::Columns(1);
				ImGui::PopID();
			}
		}
		
		ImGui::PopStyleVar();
		ImGui::Spacing();
	}
}
