/**
* @file CameraComponent.cpp.
* @brief The CameraComponent Class Implementation.
* @author Spiecs.
*/

#include "Pchheader.h"
#include "DirectionalLightComponent.h"

#include "imgui_internal.h"
#include <glm/gtc/type_ptr.hpp>

namespace Spiecs {

	void DirectionalLightComponent::OnSerialize()
	{
		
	}
	void DirectionalLightComponent::OnDeSerialize()
	{
		
	}

	void DirectionalLightComponent::DrawThis()
	{
		SPIECS_PROFILE_ZONE;

		ImGui::Spacing();
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 3.0f });
		float columeWidth = ImGuiH::GetLineItemSize().x * 6.5f;

		{
			SPIECS_PROFILE_ZONEN("DirectionalLightComponent Color");
			
			ImGuiH::DrawPropertyItem("Color", columeWidth, [&]() {
			
				float itemWidth = (ImGui::GetContentRegionAvail().x - 8.0f * ImGuiH::GetLineItemSize().x) / 3.0f;

				{
					SPIECS_PROFILE_ZONEN("DirectionalLightComponent Color R");
					ImGui::PushID("DirectionalLightComponent Color R");

					ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
					ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f, 0.2f, 0.2f, 1.0f });
					ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
					if (ImGui::Button("R", ImGuiH::GetLineItemSize())) m_DirectionalLight.color.x = 1.0f;
					ImGui::PopStyleColor(3);
					ImGui::SameLine();
					ImGui::PushItemWidth(itemWidth);
					ImGui::SliderFloat("##R", &m_DirectionalLight.color.x, 0.0f, 1.0f, "%.2f", ImGuiSliderFlags_AlwaysClamp);
					ImGui::PopItemWidth();
					ImGui::SameLine();
					if (ImGuiH::DrawResetIcon(m_DirectionalLight.color.x != 1.0f)) { m_DirectionalLight.color.x = 1.0f; };
					ImGui::SameLine();
					ImGui::PopID();
				}

				{
					SPIECS_PROFILE_ZONEN("DirectionalLightComponent Color G");
					ImGui::PushID("DirectionalLightComponent Color G");

					ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
					ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.3f, 0.8f, 0.3f, 1.0f });
					ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
					if (ImGui::Button("G", ImGuiH::GetLineItemSize())) m_DirectionalLight.color.y = 1.0f;
					ImGui::PopStyleColor(3);
					ImGui::SameLine();
					ImGui::PushItemWidth(itemWidth);
					ImGui::SliderFloat("##G", &m_DirectionalLight.color.y, 0.0f, 1.0f, "%.2f", ImGuiSliderFlags_AlwaysClamp);
					ImGui::PopItemWidth();
					ImGui::SameLine();
					if (ImGuiH::DrawResetIcon(m_DirectionalLight.color.y != 1.0f)) { m_DirectionalLight.color.y = 1.0f; };
					ImGui::SameLine();
					ImGui::PopID();
				}

				{
					SPIECS_PROFILE_ZONEN("DirectionalLightComponent Color B");
					ImGui::PushID("DirectionalLightComponent Color B");

					ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
					ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.2f, 0.35f, 0.9f, 1.0f });
					ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
					if (ImGui::Button("B", ImGuiH::GetLineItemSize())) m_DirectionalLight.color.z = 1.0f;
					ImGui::PopStyleColor(3);
					ImGui::SameLine();
					ImGui::PushItemWidth(itemWidth);
					ImGui::SliderFloat("##B", &m_DirectionalLight.color.z, 0.0f, 1.0f, "%.2f", ImGuiSliderFlags_AlwaysClamp);
					ImGui::PopItemWidth();
					ImGui::SameLine();
					if (ImGuiH::DrawResetIcon(m_DirectionalLight.color.z != 1.0f)) { m_DirectionalLight.color.z = 1.0f; };
					ImGui::SameLine();
					ImGui::PopID();
				}

				{
					SPIECS_PROFILE_ZONEN("DirectionalLightComponent Color Picker");
					ImGui::PushID("DirectionalLightComponent Color Picker");

					ImGui::PushStyleColor(ImGuiCol_FrameBg, ImGui::GetStyle().Colors[ImGuiCol_WindowBg]);
					ImGui::ColorEdit3("##", glm::value_ptr(m_DirectionalLight.color), ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_PickerHueBar);
					ImGui::SameLine();
					if (ImGuiH::DrawResetIcon(m_DirectionalLight.color != glm::vec3(1.0f, 1.0f, 1.0f))) { m_DirectionalLight.color = glm::vec3(1.0f, 1.0f, 1.0f); };
					ImGui::PopStyleColor();
					ImGui::PopID();
				}
			});
		}

		{
			SPIECS_PROFILE_ZONEN("DirectionalLightComponent Intensity");

			ImGuiH::DrawPropertyItem("Intensity", columeWidth, [&]() {
				ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x - ImGuiH::GetLineItemSize().x);
				ImGui::DragFloat("##", &m_DirectionalLight.intensity, 0.1f, 0.0f, 10000.0f, "%.2f", ImGuiSliderFlags_AlwaysClamp);
				ImGui::PopItemWidth();
				ImGui::SameLine();
				if (ImGuiH::DrawResetIcon(m_DirectionalLight.intensity != 1.0f)) { m_DirectionalLight.intensity = 1.0f; };
			});
		}

		ImGui::PopStyleVar();
		ImGui::Spacing();
	}
}
