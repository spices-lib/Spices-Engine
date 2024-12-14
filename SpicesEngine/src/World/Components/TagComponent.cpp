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

	void TagComponent::RemoveTag(const std::string& tag)
	{
		SPICES_PROFILE_ZONE;

		assert(m_Tags.size() > 0);

		if (m_Tags.size() == 1)
		{
			std::stringstream ss;
			ss << "Remove tag failed: Entity " << *m_Tags.begin() << " must have at lease one tag.";

			SPICES_CORE_WARN(ss.str())

			return;
		}

		if (m_Tags.find(tag) == m_Tags.end())
		{
			std::stringstream ss;
			ss << "Remove tag failed: Entity " << *m_Tags.begin() << " do not have tag of " << tag;

			SPICES_CORE_WARN(ss.str())

			return;
		}
			
		m_Tags.erase(tag);
	}

	void TagComponent::Rename(const std::string& tag)
	{
		SPICES_PROFILE_ZONE;

		assert(m_Tags.size() > 0);

		m_Tags.erase(*m_Tags.begin());
		m_Tags.insert(tag);
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
