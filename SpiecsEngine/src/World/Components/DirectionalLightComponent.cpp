/**
* @file CameraComponent.cpp.
* @brief The CameraComponent Class Implementation.
* @author Spiecs.
*/

#include "Pchheader.h"
#include "DirectionalLightComponent.h"

#include "imgui_internal.h"

namespace Spiecs {

	void DirectionalLightComponent::OnSerialize()
	{
		
	}
	void DirectionalLightComponent::OnDeSerialize()
	{
		
	}

	void DirectionalLightComponent::DrawThis()
	{
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 3.0f });
		static float columeWidth = ImGuiH::GetLineItemSize().x * 6.5f;
		
		{
			SPIECS_PROFILE_ZONEN("DirectionalLightComponent Color");
			
			ImGui::PushID("DLight Color");
			ImGui::Columns(2);
			
			ImGui::SetColumnWidth(0, columeWidth);
			ImGui::Text("Color");
			ImGui::NextColumn();
			
			ImGui::PushMultiItemsWidths(4, ImGui::CalcItemWidth());

			{
				SPIECS_PROFILE_ZONEN("DirectionalLightComponent Color R");
				
				ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
				ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f, 0.2f, 0.2f, 1.0f });
				ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
				if (ImGui::Button("R", ImGuiH::GetLineItemSize())) m_DirectionalLight.color.x = 1.0f;
				ImGui::PopStyleColor(3);
				ImGui::SameLine();
				ImGui::DragFloat("##R", &m_DirectionalLight.color.x, 0.002f, 0.0f, 1.0f, "%.2f", ImGuiSliderFlags_AlwaysClamp);
				ImGui::SameLine();
				ImGui::Button(ICON_MD_LOCK);
				ImGui::PopItemWidth();
				ImGui::SameLine();
			}

			{
				SPIECS_PROFILE_ZONEN("DirectionalLightComponent Color G");
				
				ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
				ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.3f, 0.8f, 0.3f, 1.0f });
				ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
				if (ImGui::Button("G", ImGuiH::GetLineItemSize())) m_DirectionalLight.color.y = 1.0f;
				ImGui::PopStyleColor(3);
				ImGui::SameLine();
				ImGui::DragFloat("##G", &m_DirectionalLight.color.y, 0.002f, 0.0f, 1.0f, "%.2f", ImGuiSliderFlags_AlwaysClamp);
				ImGui::SameLine();
				ImGui::Button(ICON_MD_LOCK);
				ImGui::PopItemWidth();
				ImGui::SameLine();
			}

			{
				SPIECS_PROFILE_ZONEN("DirectionalLightComponent Color B");
				
				ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
				ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.2f, 0.35f, 0.9f, 1.0f });
				ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
				if (ImGui::Button("B", ImGuiH::GetLineItemSize())) m_DirectionalLight.color.z = 1.0f;
				ImGui::PopStyleColor(3);
				ImGui::SameLine();
				ImGui::DragFloat("##B", &m_DirectionalLight.color.z, 0.002f, 0.0f, 1.0f, "%.2f", ImGuiSliderFlags_AlwaysClamp);
				ImGui::SameLine();
				ImGui::Button(ICON_MD_LOCK_OPEN);
				ImGui::PopItemWidth();
				ImGui::SameLine();
			}

			{
				SPIECS_PROFILE_ZONEN("DirectionalLightComponent Color Picker");
				
				ImGui::PushStyleColor(ImGuiCol_FrameBg, ImGui::GetStyle().Colors[ImGuiCol_WindowBg]);
				static float lightColor[3] = { m_DirectionalLight.color.x, m_DirectionalLight.color.y, m_DirectionalLight.color.z };
				if(ImGui::ColorEdit3("##", lightColor, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_PickerHueBar))
				{
					m_DirectionalLight.color.x = lightColor[0];
					m_DirectionalLight.color.y = lightColor[1];
					m_DirectionalLight.color.z = lightColor[2];
				}
				ImGui::SameLine();
				ImGui::Button(ICON_MD_LOCK_OPEN);
				ImGui::PopItemWidth();
				ImGui::PopStyleColor();
			}
			
			ImGui::Columns(1);
			ImGui::PopID();
		}

		{
			SPIECS_PROFILE_ZONEN("DirectionalLightComponent Intensity");
			
			ImGui::PushID("DLight Intensity");
			ImGui::Columns(2);
			
			ImGui::SetColumnWidth(0, columeWidth);
			ImGui::Text("Intensity");
			ImGui::NextColumn();
			
			ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x - ImGuiH::GetLineItemSize().x);
			ImGui::DragFloat("##", &m_DirectionalLight.intensity, 0.1f, 0.0f, 10000.0f, "%.2f", ImGuiSliderFlags_AlwaysClamp);
			ImGui::PopItemWidth();
			ImGui::SameLine();
			ImGui::Button(ICON_MD_LOCK_OPEN);
			
			ImGui::Columns(1);
			ImGui::PopID();
		}

		ImGui::PopStyleVar();
	}
}
