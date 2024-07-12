/**
* @file CameraComponent.cpp.
* @brief The CameraComponent Class Implementation.
* @author Spiecs.
*/

#include "Pchheader.h"
#include "CameraComponent.h"

namespace Spiecs {

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
		SPIECS_PROFILE_ZONE;

		ImGui::Spacing();
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 3.0f });
		float columeWidth = ImGuiH::GetLineItemSize().x * 6.5f;

		{
			SPIECS_PROFILE_ZONEN("CameraComponent Projection Type");

			
		}

		{
			SPIECS_PROFILE_ZONEN("CameraComponent Intensity");
			
			/*ImGui::PushID("DLight Intensity");
			ImGui::Columns(2, 0, false);
			
			ImGui::SetColumnWidth(0, columeWidth);
			ImGui::Text("FOV");
			ImGui::NextColumn();
			
			ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x - ImGuiH::GetLineItemSize().x);
			ImGui::DragFloat("##", &m_Camera->GetPerspectiveParam().fov, 0.1f, 1.0f, 179.0f, "%.2f", ImGuiSliderFlags_AlwaysClamp);
			ImGui::PopItemWidth();
			ImGui::SameLine();
			if (ImGuiH::DrawResetIcon(m_Camera->GetPerspectiveParam().fov != 45.0f)) { m_Camera->GetPerspectiveParam().fov = 45.0f; }

			ImGui::Columns(1);
			ImGui::PopID();*/

			auto t = []() { std::cout << "Hello" << std::endl; return true; };

			/*ImGuiH::DrawPropertyItem("FOV", columeWidth, [&](){
				ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x - ImGuiH::GetLineItemSize().x);
				ImGui::DragFloat("##", &m_Camera->GetPerspectiveParam().fov, 0.1f, 1.0f, 179.0f, "%.2f", ImGuiSliderFlags_AlwaysClamp);
				ImGui::PopItemWidth();
				ImGui::SameLine();
				if (ImGuiH::DrawResetIcon(m_Camera->GetPerspectiveParam().fov != 45.0f)) { m_Camera->GetPerspectiveParam().fov = 45.0f; }

				return true;
			});*/
			ImGuiH::DrawPropertyItem("FOV", columeWidth, t);
		}
		
		ImGui::PopStyleVar();
		ImGui::Spacing();
	}
}
