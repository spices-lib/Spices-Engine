/**
* @file CameraComponent.cpp.
* @brief The CameraComponent Class Implementation.
* @author Spices.
*/

#include "Pchheader.h"
#include "CameraComponent.h"

namespace Spices {

	CameraComponent::CameraComponent(bool isActive)
		: m_IsActive(isActive)
	{}

	void CameraComponent::OnSerialize()
	{

	}

	void CameraComponent::OnDeSerialize()
	{

	}

	void CameraComponent::DrawThis()
	{
		SPICES_PROFILE_ZONE;

		ImGui::Spacing();
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 3.0f });
		float columeWidth = ImGuiH::GetLineItemSize().x * 6.5f;

		{
			SPICES_PROFILE_ZONEN("CameraComponent FOV");

			ImGuiH::DrawPropertyItem("FOV", columeWidth, [&](){
				ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x - ImGuiH::GetLineItemSize().x);
				ImGui::DragFloat("##", &m_Camera->GetPerspectiveParam().fov, 0.1f, 1.0f, 179.0f, "%.2f", ImGuiSliderFlags_AlwaysClamp);
				ImGui::PopItemWidth();
				ImGui::SameLine();
				if (ImGuiH::DrawResetIcon(m_Camera->GetPerspectiveParam().fov != 45.0f)) { m_Camera->GetPerspectiveParam().fov = 45.0f; }
			});
		}
		ImGui::PopStyleVar();
		ImGui::Spacing();
	}
}
