/**
* @file CameraComponent.cpp.
* @brief The CameraComponent Class Implementation.
* @author Spices.
*/

#include "Pchheader.h"
#include "CameraComponent.h"
#include "Render/FrameInfo.h"
#include "World/World/World.h"

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
				if(ImGui::DragFloat("##", &m_Camera->GetPerspectiveParam().fov, 0.1f, 1.0f, 179.0f, "%.2f", ImGuiSliderFlags_AlwaysClamp))
				{
					FrameInfo::Get().m_World->Mark(World::FrushStableFrame);
				}
				ImGui::PopItemWidth();
				ImGui::SameLine();
				if (ImGuiH::DrawResetIcon(m_Camera->GetPerspectiveParam().fov != 45.0f)) 
				{
					m_Camera->GetPerspectiveParam().fov = 45.0f; 
					FrameInfo::Get().m_World->Mark(World::FrushStableFrame);
				}
			});
		}

		{
			SPICES_PROFILE_ZONEN("CameraComponent Near Plane");

			ImGuiH::DrawPropertyItem("Near Plane", columeWidth, [&]() {
				ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x - ImGuiH::GetLineItemSize().x);
				if (ImGui::DragFloat("##", &m_Camera->GetPerspectiveParam().nearPlane, 0.1f, 0.01f, 1000.0f, "%.3f", ImGuiSliderFlags_AlwaysClamp))
				{
					FrameInfo::Get().m_World->Mark(World::FrushStableFrame);
				}
				ImGui::PopItemWidth();
				ImGui::SameLine();
				if (ImGuiH::DrawResetIcon(m_Camera->GetPerspectiveParam().nearPlane != 0.01f)) 
				{ 
					m_Camera->GetPerspectiveParam().nearPlane = 0.01f; 
					FrameInfo::Get().m_World->Mark(World::FrushStableFrame);
				}
			});
		}

		{
			SPICES_PROFILE_ZONEN("CameraComponent Far Plane");

			ImGuiH::DrawPropertyItem("Far Plane", columeWidth, [&]() {
				ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x - ImGuiH::GetLineItemSize().x);
				if (ImGui::DragFloat("##", &m_Camera->GetPerspectiveParam().farPlane, 10.0f, 1000.0f, 100000.0f, "%.2f", ImGuiSliderFlags_AlwaysClamp))
				{
					FrameInfo::Get().m_World->Mark(World::FrushStableFrame);
				}
				ImGui::PopItemWidth();
				ImGui::SameLine();
				if (ImGuiH::DrawResetIcon(m_Camera->GetPerspectiveParam().farPlane != 1000.0f)) 
				{ 
					m_Camera->GetPerspectiveParam().farPlane = 1000.0f; 
					FrameInfo::Get().m_World->Mark(World::FrushStableFrame);
				}
			});
		}

		{
			SPICES_PROFILE_ZONEN("CameraComponent AspectRatio");

			ImGuiH::DrawPropertyItem("AspectRatio", columeWidth, [&]() {
				ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x - ImGuiH::GetLineItemSize().x);
				float aspectRatio = m_Camera->GetPerspectiveParam().aspectRatio;
				ImGui::DragFloat("##", &aspectRatio, 10.0f, 0.0f, 100.0f, "%.3f", ImGuiSliderFlags_AlwaysClamp);
				ImGui::PopItemWidth();
				ImGui::SameLine();
				ImGuiH::DrawResetIcon(m_Camera->GetPerspectiveParam().aspectRatio != 1.777f);
			});
		}

		ImGui::PopStyleVar();
		ImGui::Spacing();
	}
}
