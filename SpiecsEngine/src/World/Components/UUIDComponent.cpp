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

			ImGuiH::DrawPropertyItem("UUID", columeWidth, [&]() {
				ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x - ImGuiH::GetLineItemSize().x);
				char buffer[256] = {};
				strcpy_s(buffer, sizeof(buffer), std::to_string(m_uuid).c_str());
				if (ImGui::InputText("##", buffer, sizeof(buffer)))
				{
					strcpy_s(buffer, sizeof(buffer), std::to_string(m_uuid).c_str());
				}
				ImGui::PopItemWidth();
				ImGui::SameLine();
				ImGuiH::DrawResetIcon(false);
			});
		}
		
		ImGui::PopStyleVar();
		ImGui::Spacing();
	}
}
