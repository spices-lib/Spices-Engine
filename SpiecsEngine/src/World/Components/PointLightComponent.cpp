/**
* @file PointLightComponent.cpp.
* @brief The PointLightComponent Class Implementation.
* @author Spiecs.
*/

#include "Pchheader.h"
#include "PointLightComponent.h"

#include <imgui_internal.h>
#include <glm/gtc/type_ptr.hpp>

namespace Spiecs {
	
	void PointLightComponent::OnSerialize()
	{
	}
	
	void PointLightComponent::OnDeSerialize()
	{
	}

	void PointLightComponent::DrawThis()
	{
		SPIECS_PROFILE_ZONE;

		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 3.0f });
		float columeWidth = ImGuiH::GetLineItemSize().x * 6.5f;
		
		{
			SPIECS_PROFILE_ZONEN("PointLightComponent Color");
			
			ImGui::PushID("PointLightComponent Color");
			ImGui::Columns(2);
			
			ImGui::SetColumnWidth(0, columeWidth);
			ImGui::Text("Color");
			ImGui::NextColumn();
			
			float itemWidth = (ImGui::GetContentRegionAvail().x - 8.0f * ImGuiH::GetLineItemSize().x) / 3.0f;

			{
				SPIECS_PROFILE_ZONEN("PointLightComponent Color R");
				
				ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
				ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f, 0.2f, 0.2f, 1.0f });
				ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
				if (ImGui::Button("R", ImGuiH::GetLineItemSize())) m_PointLight.color.x = 1.0f;
				ImGui::PopStyleColor(3);
				ImGui::SameLine();
				ImGui::PushItemWidth(itemWidth);
				ImGui::DragFloat("##R", &m_PointLight.color.x, 0.002f, 0.0f, 1.0f, "%.2f", ImGuiSliderFlags_AlwaysClamp);
				ImGui::PopItemWidth();
				ImGui::SameLine();
				ImGui::Button(ICON_MD_LOCK, ImGuiH::GetLineItemSize());
				ImGui::SameLine();
			}

			{
				SPIECS_PROFILE_ZONEN("PointLightComponent Color G");
				
				ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
				ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.3f, 0.8f, 0.3f, 1.0f });
				ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
				if (ImGui::Button("G", ImGuiH::GetLineItemSize())) m_PointLight.color.y = 1.0f;
				ImGui::PopStyleColor(3);
				ImGui::SameLine();
				ImGui::PushItemWidth(itemWidth);
				ImGui::DragFloat("##G", &m_PointLight.color.y, 0.002f, 0.0f, 1.0f, "%.2f", ImGuiSliderFlags_AlwaysClamp);
				ImGui::PopItemWidth();
				ImGui::SameLine();
				ImGui::Button(ICON_MD_LOCK, ImGuiH::GetLineItemSize());
				ImGui::SameLine();
			}

			{
				SPIECS_PROFILE_ZONEN("PointLightComponent Color B");
				
				ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
				ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.2f, 0.35f, 0.9f, 1.0f });
				ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
				if (ImGui::Button("B", ImGuiH::GetLineItemSize())) m_PointLight.color.z = 1.0f;
				ImGui::PopStyleColor(3);
				ImGui::SameLine();
				ImGui::PushItemWidth(itemWidth);
				ImGui::DragFloat("##B", &m_PointLight.color.z, 0.002f, 0.0f, 1.0f, "%.2f", ImGuiSliderFlags_AlwaysClamp);
				ImGui::PopItemWidth();
				ImGui::SameLine();
				ImGui::Button(ICON_MD_LOCK_OPEN, ImGuiH::GetLineItemSize());
				ImGui::SameLine();
			}

			{
				SPIECS_PROFILE_ZONEN("PointLightComponent Color Picker");
				
				ImGui::PushStyleColor(ImGuiCol_FrameBg, ImGui::GetStyle().Colors[ImGuiCol_WindowBg]);
				ImGui::ColorEdit3("##", glm::value_ptr(m_PointLight.color), ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_PickerHueBar);
				ImGui::SameLine();
				ImGui::Button(ICON_MD_LOCK_OPEN, ImGuiH::GetLineItemSize());
				ImGui::PopStyleColor();
			}
			
			ImGui::Columns(1);
			ImGui::PopID();
		}

		{
			SPIECS_PROFILE_ZONEN("PointLightComponent Intensity");
			
			ImGui::PushID("Intensity");
			ImGui::Columns(2);
			
			ImGui::SetColumnWidth(0, columeWidth);
			ImGui::Text("Intensity");
			ImGui::NextColumn();
			
			ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x - ImGuiH::GetLineItemSize().x);
			ImGui::DragFloat("##", &m_PointLight.intensity, 0.1f, 0.0f, 10000.0f, "%.2f", ImGuiSliderFlags_AlwaysClamp);
			ImGui::PopItemWidth();
			ImGui::SameLine();
			ImGui::Button(ICON_MD_LOCK_OPEN);
			
			ImGui::Columns(1);
			ImGui::PopID();
		}

		{
			SPIECS_PROFILE_ZONEN("PointLightComponent Constantf");
			
			ImGui::PushID("Constantf");
			ImGui::Columns(2);
			
			ImGui::SetColumnWidth(0, columeWidth);
			ImGui::Text("Constantf");
			ImGui::NextColumn();
			
			ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x - ImGuiH::GetLineItemSize().x);
			ImGui::DragFloat("##", &m_PointLight.constantf, 0.1f, 0.0f, 10000.0f, "%.2f", ImGuiSliderFlags_AlwaysClamp);
			ImGui::PopItemWidth();
			ImGui::SameLine();
			ImGui::Button(ICON_MD_LOCK_OPEN);
			
			ImGui::Columns(1);
			ImGui::PopID();
		}

		{
			SPIECS_PROFILE_ZONEN("PointLightComponent Linear");
			
			ImGui::PushID("Linear");
			ImGui::Columns(2);
			
			ImGui::SetColumnWidth(0, columeWidth);
			ImGui::Text("Linear");
			ImGui::NextColumn();
			
			ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x - ImGuiH::GetLineItemSize().x);
			ImGui::DragFloat("##", &m_PointLight.linear, 0.1f, 0.0f, 10000.0f, "%.2f", ImGuiSliderFlags_AlwaysClamp);
			ImGui::PopItemWidth();
			ImGui::SameLine();
			ImGui::Button(ICON_MD_LOCK_OPEN);
			
			ImGui::Columns(1);
			ImGui::PopID();
		}

		{
			SPIECS_PROFILE_ZONEN("PointLightComponent Quadratic");
			
			ImGui::PushID("Quadratic");
			ImGui::Columns(2);
			
			ImGui::SetColumnWidth(0, columeWidth);
			ImGui::Text("Quadratic");
			ImGui::NextColumn();
			
			ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x - ImGuiH::GetLineItemSize().x);
			ImGui::DragFloat("##", &m_PointLight.quadratic, 0.1f, 0.0f, 10000.0f, "%.2f", ImGuiSliderFlags_AlwaysClamp);
			ImGui::PopItemWidth();
			ImGui::SameLine();
			ImGui::Button(ICON_MD_LOCK_OPEN);
			
			ImGui::Columns(1);
			ImGui::PopID();
		}

		ImGui::PopStyleVar();
	}
}
