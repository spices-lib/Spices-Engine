/**
* @file UUIDComponent.cpp.
* @brief The UUIDComponent Class Implementation.
* @author Spices.
*/

#include "Pchheader.h"
#include "UUIDComponent.h"

namespace Spices {

	void UUIDComponent::OnSerialize()
	{

	}

	void UUIDComponent::OnDeSerialize()
	{

	}

	void UUIDComponent::DrawThis()
	{
		SPICES_PROFILE_ZONE;
		
		ImGui::Spacing();
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 3.0f });
		float columeWidth = ImGuiH::GetLineItemSize().x * 6.5f;

		{
			SPICES_PROFILE_ZONEN("UUIDComponent UUID");

			ImGuiH::DrawPropertyItem("UUID", columeWidth, nullptr, [&]() {
				ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x - ImGuiH::GetLineItemSize().x);
				char buffer[256] = {};
				ImGui::InputTextWithHint("##", ICON_TEXT_ROW(" ", std::to_string(m_uuid).c_str()), buffer, sizeof(buffer));
				ImGui::PopItemWidth();
				ImGui::SameLine();
				ImGuiH::DrawResetIcon(false);
			});
		}
		
		ImGui::PopStyleVar();
		ImGui::Spacing();
	}
}
