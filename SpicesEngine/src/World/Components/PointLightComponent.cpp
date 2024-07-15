/**
* @file PointLightComponent.cpp.
* @brief The PointLightComponent Class Implementation.
* @author Spices.
*/

#include "Pchheader.h"
#include "PointLightComponent.h"
#include "Render/FrameInfo.h"
#include "World/World/World.h"

#include <imgui_internal.h>
#include <glm/gtc/type_ptr.hpp>

namespace Spices {
	
	void PointLightComponent::OnSerialize()
	{
	}
	
	void PointLightComponent::OnDeSerialize()
	{
	}

	void PointLightComponent::DrawThis()
	{
		SPICES_PROFILE_ZONE;

		ImGui::Spacing();
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 3.0f });
		float columeWidth = ImGuiH::GetLineItemSize().x * 6.5f;
		
		{
			SPICES_PROFILE_ZONEN("PointLightComponent Color");
			
			ImGuiH::DrawPropertyItem("Color", columeWidth, [&]() {

				float itemWidth = (ImGui::GetContentRegionAvail().x - 8.0f * ImGuiH::GetLineItemSize().x) / 3.0f;

				{
					SPICES_PROFILE_ZONEN("PointLightComponent Color R");
					ImGui::PushID("PointLightComponent Color R");

					ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
					ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f, 0.2f, 0.2f, 1.0f });
					ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
					if (ImGui::Button("R", ImGuiH::GetLineItemSize())) m_PointLight.color.x = 1.0f;
					ImGui::PopStyleColor(3);
					ImGui::SameLine();
					ImGui::PushItemWidth(itemWidth);
					if(ImGui::SliderFloat("##R", &m_PointLight.color.x, 0.0f, 1.0f, "%.2f", ImGuiSliderFlags_AlwaysClamp))
					{
						FrameInfo::Get().m_World->Mark(FrushStableFrame);
					}
					ImGui::PopItemWidth();
					ImGui::SameLine();
					if (ImGuiH::DrawResetIcon(m_PointLight.color.x != 1.0f)) 
					{ 
						m_PointLight.color.x = 1.0f;
						FrameInfo::Get().m_World->Mark(FrushStableFrame);
					}
					ImGui::SameLine();
					ImGui::PopID();
				}

				{
					SPICES_PROFILE_ZONEN("PointLightComponent Color G");
					ImGui::PushID("PointLightComponent Color G");
				
					ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
					ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.3f, 0.8f, 0.3f, 1.0f });
					ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
					if (ImGui::Button("G", ImGuiH::GetLineItemSize())) m_PointLight.color.y = 1.0f;
					ImGui::PopStyleColor(3);
					ImGui::SameLine();
					ImGui::PushItemWidth(itemWidth);
					if(ImGui::SliderFloat("##G", &m_PointLight.color.y, 0.0f, 1.0f, "%.2f", ImGuiSliderFlags_AlwaysClamp))
					{
						FrameInfo::Get().m_World->Mark(FrushStableFrame);
					}
					ImGui::PopItemWidth();
					ImGui::SameLine();
					if (ImGuiH::DrawResetIcon(m_PointLight.color.y != 1.0f)) 
					{ 
						m_PointLight.color.y = 1.0f;
						FrameInfo::Get().m_World->Mark(FrushStableFrame);
					}
					ImGui::SameLine();
					ImGui::PopID();
				}

				{
					SPICES_PROFILE_ZONEN("PointLightComponent Color B");
					ImGui::PushID("PointLightComponent Color B");
				
					ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
					ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.2f, 0.35f, 0.9f, 1.0f });
					ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
					if (ImGui::Button("B", ImGuiH::GetLineItemSize())) m_PointLight.color.z = 1.0f;
					ImGui::PopStyleColor(3);
					ImGui::SameLine();
					ImGui::PushItemWidth(itemWidth);
					if(ImGui::SliderFloat("##B", &m_PointLight.color.z, 0.0f, 1.0f, "%.2f", ImGuiSliderFlags_AlwaysClamp))
					{
						FrameInfo::Get().m_World->Mark(FrushStableFrame);
					}
					ImGui::PopItemWidth();
					ImGui::SameLine();
					if (ImGuiH::DrawResetIcon(m_PointLight.color.z != 1.0f)) 
					{
						m_PointLight.color.z = 1.0f; 
						FrameInfo::Get().m_World->Mark(FrushStableFrame);
					};
					ImGui::SameLine();
					ImGui::PopID();
				}

				{
					SPICES_PROFILE_ZONEN("PointLightComponent Color Picker");
					ImGui::PushID("PointLightComponent Color Picker");
				
					ImGui::PushStyleColor(ImGuiCol_FrameBg, ImGui::GetStyle().Colors[ImGuiCol_WindowBg]);
					ImGui::ColorEdit3("##", glm::value_ptr(m_PointLight.color), ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_PickerHueBar);
					ImGui::SameLine();
					if (ImGuiH::DrawResetIcon(m_PointLight.color != glm::vec3(1.0f, 1.0f, 1.0f))) 
					{ 
						m_PointLight.color = glm::vec3(1.0f, 1.0f, 1.0f);
						FrameInfo::Get().m_World->Mark(FrushStableFrame);
					};
					ImGui::PopStyleColor();
					ImGui::PopID();
				}
			});
		}

		{
			SPICES_PROFILE_ZONEN("PointLightComponent Intensity");

			ImGuiH::DrawPropertyItem("Intensity", columeWidth, [&]() {
				ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x - ImGuiH::GetLineItemSize().x);
				if(ImGui::DragFloat("##", &m_PointLight.intensity, 0.1f, 0.0f, 10000.0f, "%.2f", ImGuiSliderFlags_AlwaysClamp))
				{
					FrameInfo::Get().m_World->Mark(FrushStableFrame);
				}
				ImGui::PopItemWidth();
				ImGui::SameLine();
				if (ImGuiH::DrawResetIcon(m_PointLight.intensity != 1.0f)) 
				{
					m_PointLight.intensity = 1.0f; 
					FrameInfo::Get().m_World->Mark(FrushStableFrame);
				};
			});
		}

		{
			SPICES_PROFILE_ZONEN("PointLightComponent Constantf");

			ImGuiH::DrawPropertyItem("Constantf", columeWidth, [&]() {
				ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x - ImGuiH::GetLineItemSize().x);
				if(ImGui::DragFloat("##", &m_PointLight.constantf, 0.1f, 0.0f, 10000.0f, "%.2f", ImGuiSliderFlags_AlwaysClamp))
				{
					FrameInfo::Get().m_World->Mark(FrushStableFrame);
				}
				ImGui::PopItemWidth();
				ImGui::SameLine();
				if (ImGuiH::DrawResetIcon(m_PointLight.constantf != 1.0f)) 
				{ 
					m_PointLight.constantf = 1.0f; 
					FrameInfo::Get().m_World->Mark(FrushStableFrame);
				};
			});
		}

		{
			SPICES_PROFILE_ZONEN("PointLightComponent Linear");

			ImGuiH::DrawPropertyItem("Linear", columeWidth, [&]() {
				ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x - ImGuiH::GetLineItemSize().x);
				if(ImGui::DragFloat("##", &m_PointLight.linear, 0.1f, 0.0f, 10000.0f, "%.2f", ImGuiSliderFlags_AlwaysClamp))
				{
					FrameInfo::Get().m_World->Mark(FrushStableFrame);
				}
				ImGui::PopItemWidth();
				ImGui::SameLine();
				if (ImGuiH::DrawResetIcon(m_PointLight.linear != 0.35f)) 
				{
					m_PointLight.linear = 0.35f; 
					FrameInfo::Get().m_World->Mark(FrushStableFrame);
				};
			});
		}

		{
			SPICES_PROFILE_ZONEN("PointLightComponent Quadratic");

			ImGuiH::DrawPropertyItem("Linear", columeWidth, [&]() {
				ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x - ImGuiH::GetLineItemSize().x);
				if(ImGui::DragFloat("##", &m_PointLight.quadratic, 0.1f, 0.0f, 10000.0f, "%.2f", ImGuiSliderFlags_AlwaysClamp))
				{
					FrameInfo::Get().m_World->Mark(FrushStableFrame);
				}
				ImGui::PopItemWidth();
				ImGui::SameLine();
				if (ImGuiH::DrawResetIcon(m_PointLight.quadratic != 0.44f)) 
				{ 
					m_PointLight.quadratic = 0.44f; 
					FrameInfo::Get().m_World->Mark(FrushStableFrame);
				};
			});
		}

		ImGui::PopStyleVar();
		ImGui::Spacing();
	}
}
