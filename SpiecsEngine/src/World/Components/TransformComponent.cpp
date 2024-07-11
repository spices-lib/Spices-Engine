/**
* @file TransformComponent.cpp.
* @brief The TransformComponent Class Implementation.
* @author Spiecs.
*/

#include "Pchheader.h"
#include "TransformComponent.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

#include <imgui_internal.h>

namespace Spiecs {

	void TransformComponent::OnSerialize()
	{

	}

	void TransformComponent::OnDeSerialize()
	{

	}

	void TransformComponent::DrawThis()
	{
		SPIECS_PROFILE_ZONE;

		ImGui::Spacing();
		
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 3.0f });
		
		/**
		* @brief colume_0 width.
		*/
		const float columeWidth = ImGuiH::GetLineItemSize().x * 5.0f;

		/**
		* @brief drag float width, 3 * drag float + 6 * buttom + colume_0 = ImGui::GetContentRegionAvail().
		*/
		const float itemWidth = (ImGui::GetContentRegionAvail().x - 11.0f * ImGuiH::GetLineItemSize().x) / 3.0f;

		{
			SPIECS_PROFILE_ZONEN("TransformComponent Translate");

			ImGui::PushID("TransformComponent Translate");

			ImGui::Columns(2);
			ImGui::SetColumnWidth(0, columeWidth);
			
			static bool selected = false;
			if (ImGui::Selectable("Translate", selected)) selected = !selected;
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(4.0f, 4.0f));
			ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(6.0f, 6.0f));
			if (ImGui::BeginPopupContextItem()) // <-- use last item id as popup id
			{
				ImGui::MenuItem("Copy");
				ImGui::MenuItem("Paste");
				ImGui::MenuItem("Copy Property Path");
				ImGui::Separator();
				ImGui::MenuItem("Disable");
				ImGui::MenuItem("Delete");
				ImGui::Separator();
				ImGui::MenuItem("Set Key");
				ImGui::MenuItem("Remove Key");
				ImGui::MenuItem("Copy Key");
				ImGui::MenuItem("Paste Key");
				ImGui::Separator();
				ImGui::MenuItem("Copy Animation");
				ImGui::MenuItem("Paste Animation");
				ImGui::MenuItem("Remove Animation");
				ImGui::Separator();
				if (ImGui::BeginMenu("Locks"))
				{
					ImGui::MenuItem("Lock");
					ImGui::MenuItem("UnLock");

					ImGui::EndMenu();
				}

				ImGui::EndPopup();
			}
			ImGui::SetItemTooltip("xformOp:translate(float3)\nRight click it to disable or delete it.");
			ImGui::PopStyleVar(2);

			ImGui::NextColumn();

			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f, 0.2f, 0.2f, 1.0f });
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
			ImGui::Button("X", ImGuiH::GetLineItemSize());
			ImGui::PopStyleColor(3);
			ImGui::SameLine();
			ImGui::PushItemWidth(itemWidth);
			ImGui::DragFloat("##X", &m_Position.x, 0.1f, 0.0f, 0.0f, "%.2f");
			ImGui::PopItemWidth();
			ImGui::SameLine();
			if (ImGuiH::DrawResetIcon(m_Position.x != 0.0f)){ m_Position.x = 0.0f; }
			ImGui::SameLine();

			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.3f, 0.8f, 0.3f, 1.0f });
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
			ImGui::Button("Y", ImGuiH::GetLineItemSize());
			ImGui::PopStyleColor(3);
			ImGui::SameLine();
			ImGui::PushItemWidth(itemWidth);
			ImGui::DragFloat("##Y", &m_Position.y, 0.1f, 0.0f, 0.0f, "%.2f");
			ImGui::PopItemWidth();
			ImGui::SameLine();
			if(ImGuiH::DrawResetIcon(m_Position.y != 0.0f)) { m_Position.y = 0.0f; };
			ImGui::SameLine();

			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.2f, 0.35f, 0.9f, 1.0f });
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
			ImGui::Button("Z", ImGuiH::GetLineItemSize());
			ImGui::PopStyleColor(3);
			ImGui::SameLine();
			ImGui::PushItemWidth(itemWidth);
			ImGui::DragFloat("##Z", &m_Position.z, 0.1f, 0.0f, 0.0f, "%.2f");
			ImGui::PopItemWidth();
			ImGui::SameLine();
			if (ImGuiH::DrawResetIcon(m_Position.z != 0.0f)) { m_Position.z = 0.0f; };

			ImGui::Columns(1);
			ImGui::PopID();
		}

		{
			SPIECS_PROFILE_ZONEN("TransformComponent Rotate");

			ImGui::PushID("TransformComponent Rotate");

			ImGui::Columns(2);
			ImGui::SetColumnWidth(0, columeWidth);
			/*if (ImGui::BeginMenu("Rotate"))
			{
				ImGui::MenuItem(ICON_TEXT(ICON_EMPTY, XYZ));
				ImGui::MenuItem(ICON_TEXT(ICON_EMPTY, XZY));
				ImGui::MenuItem(ICON_TEXT(ICON_EMPTY, YXZ));
				ImGui::MenuItem(ICON_TEXT(ICON_EMPTY, YZX));
				ImGui::MenuItem(ICON_TEXT(ICON_EMPTY, ZXY));
				ImGui::MenuItem(ICON_TEXT(ICON_EMPTY, ZYX));

				ImGui::EndMenu();
			}*/
			
			static bool selected = false;
			if (ImGui::Selectable("Rotate", selected)) selected = !selected;
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(4.0f, 4.0f));
			ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(6.0f, 6.0f));
			if (ImGui::BeginPopupContextItem()) // <-- use last item id as popup id
			{
				ImGui::MenuItem("Copy");
				ImGui::MenuItem("Paste");
				ImGui::MenuItem("Copy Property Path");
				ImGui::Separator();
				ImGui::MenuItem("Disable");
				ImGui::MenuItem("Delete");
				ImGui::Separator();
				ImGui::MenuItem("Set Key");
				ImGui::MenuItem("Remove Key");
				ImGui::MenuItem("Copy Key");
				ImGui::MenuItem("Paste Key");
				ImGui::Separator();
				ImGui::MenuItem("Copy Animation");
				ImGui::MenuItem("Paste Animation");
				ImGui::MenuItem("Remove Animation");
				ImGui::Separator();
				if (ImGui::BeginMenu("Locks"))
				{
					ImGui::MenuItem("Lock");
					ImGui::MenuItem("UnLock");

					ImGui::EndMenu();
				}

				ImGui::EndPopup();
			}
			ImGui::SetItemTooltip("xformOp:rotateXYZ(float3)\nRight click it to disable or delete it.\nLeft click it to change the rotate order, default is XYZ.");
			ImGui::PopStyleVar(2);

			ImGui::NextColumn();

			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f, 0.2f, 0.2f, 1.0f });
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
			ImGui::Button("X", ImGuiH::GetLineItemSize());
			ImGui::PopStyleColor(3);
			ImGui::SameLine();
			ImGui::PushItemWidth(itemWidth);
			ImGui::DragFloat("##X", &m_Rotation.x, 0.1f, 0.0f, 0.0f, "%.2f");
			ImGui::PopItemWidth();
			ImGui::SameLine();
			if (ImGuiH::DrawResetIcon(m_Rotation.x != 0.0f)) { m_Rotation.x = 0.0f; };
			ImGui::SameLine();

			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.3f, 0.8f, 0.3f, 1.0f });
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
			ImGui::Button("Y", ImGuiH::GetLineItemSize());
			ImGui::PopStyleColor(3);
			ImGui::SameLine();
			ImGui::PushItemWidth(itemWidth);
			ImGui::DragFloat("##Y", &m_Rotation.y, 0.1f, 0.0f, 0.0f, "%.2f");
			ImGui::PopItemWidth();
			ImGui::SameLine();
			if (ImGuiH::DrawResetIcon(m_Rotation.y != 0.0f)) { m_Rotation.y = 0.0f; };
			ImGui::SameLine();

			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.2f, 0.35f, 0.9f, 1.0f });
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
			ImGui::Button("Z", ImGuiH::GetLineItemSize());
			ImGui::PopStyleColor(3);
			ImGui::SameLine();
			ImGui::PushItemWidth(itemWidth);
			ImGui::DragFloat("##Z", &m_Rotation.z, 0.1f, 0.0f, 0.0f, "%.2f");
			ImGui::PopItemWidth();
			ImGui::SameLine();
			if (ImGuiH::DrawResetIcon(m_Rotation.z != 0.0f)) { m_Rotation.z = 0.0f; };

			ImGui::Columns(1);
			ImGui::PopID();
		}

		{
			SPIECS_PROFILE_ZONEN("TransformComponent Scale");

			ImGui::PushID("TransformComponent Scale");

			ImGui::Columns(2);
			ImGui::SetColumnWidth(0, columeWidth);
			
			static bool selected = false;
			if (ImGui::Selectable("Scale", selected)) selected = !selected;
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(4.0f, 4.0f));
			ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(6.0f, 6.0f));
			if (ImGui::BeginPopupContextItem()) // <-- use last item id as popup id
			{
				ImGui::MenuItem("Copy");
				ImGui::MenuItem("Paste");
				ImGui::MenuItem("Copy Property Path");
				ImGui::Separator();
				ImGui::MenuItem("Disable");
				ImGui::MenuItem("Delete");
				ImGui::Separator();
				ImGui::MenuItem("Set Key");
				ImGui::MenuItem("Remove Key");
				ImGui::MenuItem("Copy Key");
				ImGui::MenuItem("Paste Key");
				ImGui::Separator();
				ImGui::MenuItem("Copy Animation");
				ImGui::MenuItem("Paste Animation");
				ImGui::MenuItem("Remove Animation");
				ImGui::Separator();
				if (ImGui::BeginMenu("Locks"))
				{
					ImGui::MenuItem("Lock");
					ImGui::MenuItem("UnLock");

					ImGui::EndMenu();
				}

				ImGui::EndPopup();
			}
			ImGui::SetItemTooltip("xformOp:scale(float3)\nRight click it for more options.");
			ImGui::PopStyleVar(2);

			ImGui::SameLine();
			ImGui::Button(ICON_MD_LINK, ImGuiH::GetLineItemSize());
			ImGui::NextColumn();

			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f, 0.2f, 0.2f, 1.0f });
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
			ImGui::Button("X", ImGuiH::GetLineItemSize());
			ImGui::PopStyleColor(3);
			ImGui::SameLine();
			ImGui::PushItemWidth(itemWidth);
			ImGui::DragFloat("##X", &m_Scale.x, 0.1f, 0.0f, 0.0f, "%.2f");
			ImGui::PopItemWidth();
			ImGui::SameLine();
			if (ImGuiH::DrawResetIcon(m_Scale.x != 1.0f)) { m_Scale.x = 1.0f; };
			ImGui::SameLine();

			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.3f, 0.8f, 0.3f, 1.0f });
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
			ImGui::Button("Y", ImGuiH::GetLineItemSize());
			ImGui::PopStyleColor(3);
			ImGui::SameLine();
			ImGui::PushItemWidth(itemWidth);
			ImGui::DragFloat("##Y", &m_Scale.y, 0.1f, 0.0f, 0.0f, "%.2f");
			ImGui::PopItemWidth();
			ImGui::SameLine();
			if (ImGuiH::DrawResetIcon(m_Scale.y != 1.0f)) { m_Scale.y = 1.0f; };
			ImGui::SameLine();

			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.2f, 0.35f, 0.9f, 1.0f });
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
			ImGui::Button("Z", ImGuiH::GetLineItemSize());
			ImGui::PopStyleColor(3);
			ImGui::SameLine();
			ImGui::PushItemWidth(itemWidth);
			ImGui::DragFloat("##Z", &m_Scale.z, 0.1f, 0.0f, 0.0f, "%.2f");
			ImGui::PopItemWidth();
			ImGui::SameLine();
			if (ImGuiH::DrawResetIcon(m_Scale.z != 1.0f)) { m_Scale.z = 1.0f; };

			ImGui::Columns(1);
			ImGui::PopID();
		}
		
		ImGui::PopStyleVar();
		ImGui::Spacing();
	}

	glm::mat4 TransformComponent::GetRotateMatrix() const
	{
		/**
		* @brief Use raidans
		*/
		return glm::toMat4(glm::quat(m_Rotation));
	}

	void TransformComponent::CalMatrix()
	{
		const glm::mat4 rotation = GetRotateMatrix();
		m_ModelMatrix = glm::translate(glm::mat4(1.0f), m_Position) * rotation * glm::scale(glm::mat4(1.0f), m_Scale);
	}
	
}