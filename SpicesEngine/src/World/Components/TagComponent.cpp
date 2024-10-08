/**
* @file TagComponent.cpp.
* @brief The TagComponent Class Implementation.
* @author Spices.
*/

#include "Pchheader.h"
#include "TagComponent.h"

namespace Spices {

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
		SPICES_PROFILE_ZONE;

		std::vector<std::string> tagBuffer(m_Tags.begin(), m_Tags.end());
		
		ImGui::Spacing();
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 3.0f });
		float columeWidth = ImGuiH::GetLineItemSize().x * 6.5f;

		{
			SPICES_PROFILE_ZONEN("TagComponent Tags");
			
			for(int i = 0; i < tagBuffer.size(); i++)
			{
				std::stringstream ss;
				ss << "Tag[" << i << "]";
				ImGuiH::DrawPropertyItem(ss.str(), columeWidth, nullptr, [&]() {
					ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x - ImGuiH::GetLineItemSize().x);
					char buffer[256] = {};
					strcpy_s(buffer, sizeof(buffer), tagBuffer[i].c_str());
					if (ImGui::InputText("##", buffer, sizeof(buffer)))
					{
						m_Tags.erase(tagBuffer[i]);
						m_Tags.insert(std::string(buffer));
					}
					ImGui::PopItemWidth();
					ImGui::SameLine();
					ImGuiH::DrawResetIcon(false);
				});
			}
		}
		
		ImGui::PopStyleVar();
		ImGui::Spacing();
	}
}
