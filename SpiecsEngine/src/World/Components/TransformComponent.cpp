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
		{
			ImGui::PushID("Translate");

			ImGui::Columns(2);
			ImGui::SetColumnWidth(0, ImGuiH::GetLineItemSize().x * 5.0f);
			
			static bool selected = false;
			if (ImGui::Selectable("Translate", selected))
				selected = !selected;
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

			ImGui::NextColumn();

			ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
			ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });

			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f, 0.2f, 0.2f, 1.0f });
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
			if (ImGui::Button("X", ImGuiH::GetLineItemSize())) m_Position.x = 0.0f;
			ImGui::PopStyleColor(3);
			ImGui::SameLine();
			ImGui::DragFloat("##X", &m_Position.x, 0.1f, 0.0f, 0.0f, "%.2f");
			ImGui::SameLine();
			ImGui::Button(ICON_MD_LOCK);
			ImGui::PopItemWidth();
			ImGui::SameLine();

			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.3f, 0.8f, 0.3f, 1.0f });
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
			if (ImGui::Button("Y", ImGuiH::GetLineItemSize())) m_Position.y = 0.0f;
			ImGui::PopStyleColor(3);
			ImGui::SameLine();
			ImGui::DragFloat("##Y", &m_Position.y, 0.1f, 0.0f, 0.0f, "%.2f");
			ImGui::SameLine();
			ImGui::Button(ICON_MD_LOCK);
			ImGui::PopItemWidth();
			ImGui::SameLine();

			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.2f, 0.35f, 0.9f, 1.0f });
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
			if (ImGui::Button("Z", ImGuiH::GetLineItemSize())) m_Position.z = 0.0f;
			ImGui::PopStyleColor(3);
			ImGui::SameLine();
			ImGui::DragFloat("##Z", &m_Position.z, 0.1f, 0.0f, 0.0f, "%.2f");
			ImGui::SameLine();
			ImGui::Button(ICON_MD_LOCK_OPEN);
			ImGui::PopItemWidth();

			ImGui::PopStyleVar();
			ImGui::Columns(1);
			ImGui::PopID();
		}

		{
			ImGui::PushID("Rotate");

			ImGui::Columns(2);
			ImGui::SetColumnWidth(0, ImGuiH::GetLineItemSize().x * 5.0f);
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
			if (ImGui::Selectable("Rotate", selected))
				selected = !selected;
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

			ImGui::NextColumn();

			ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
			ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });

			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f, 0.2f, 0.2f, 1.0f });
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
			if (ImGui::Button("X", ImGuiH::GetLineItemSize())) m_Rotation.x = 0.0f;
			ImGui::PopStyleColor(3);
			ImGui::SameLine();
			ImGui::DragFloat("##X", &m_Rotation.x, 0.1f, 0.0f, 0.0f, "%.2f");
			ImGui::SameLine();
			ImGui::Button(ICON_MD_LOCK);
			ImGui::PopItemWidth();
			ImGui::SameLine();

			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.3f, 0.8f, 0.3f, 1.0f });
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
			if (ImGui::Button("Y", ImGuiH::GetLineItemSize())) m_Rotation.y = 0.0f;
			ImGui::PopStyleColor(3);
			ImGui::SameLine();
			ImGui::DragFloat("##Y", &m_Rotation.y, 0.1f, 0.0f, 0.0f, "%.2f");
			ImGui::SameLine();
			ImGui::Button(ICON_MD_LOCK);
			ImGui::PopItemWidth();
			ImGui::SameLine();

			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.2f, 0.35f, 0.9f, 1.0f });
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
			if (ImGui::Button("Z", ImGuiH::GetLineItemSize())) m_Rotation.z = 0.0f;
			ImGui::PopStyleColor(3);
			ImGui::SameLine();
			ImGui::DragFloat("##Z", &m_Rotation.z, 0.1f, 0.0f, 0.0f, "%.2f");
			ImGui::SameLine();
			ImGui::Button(ICON_MD_LOCK_OPEN);
			ImGui::PopItemWidth();

			ImGui::PopStyleVar();
			ImGui::Columns(1);
			ImGui::PopID();
		}

		{
			ImGui::PushID("Scale");

			ImGui::Columns(2);
			ImGui::SetColumnWidth(0, ImGuiH::GetLineItemSize().x * 5.0f);
			
			static bool selected = false;
			if (ImGui::Selectable("Scale", selected))
				selected = !selected;
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

			ImGui::SameLine();
			ImGui::Button(ICON_MD_LINK);
			ImGui::NextColumn();

			ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
			ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });

			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f, 0.2f, 0.2f, 1.0f });
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
			if (ImGui::Button("X", ImGuiH::GetLineItemSize())) m_Scale.x = 0.0f;
			ImGui::PopStyleColor(3);
			ImGui::SameLine();
			ImGui::DragFloat("##X", &m_Scale.x, 0.1f, 0.0f, 0.0f, "%.2f");
			ImGui::SameLine();
			ImGui::Button(ICON_MD_LOCK);
			ImGui::PopItemWidth();
			ImGui::SameLine();

			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.3f, 0.8f, 0.3f, 1.0f });
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
			if (ImGui::Button("Y", ImGuiH::GetLineItemSize())) m_Scale.y = 0.0f;
			ImGui::PopStyleColor(3);
			ImGui::SameLine();
			ImGui::DragFloat("##Y", &m_Scale.y, 0.1f, 0.0f, 0.0f, "%.2f");
			ImGui::SameLine();
			ImGui::Button(ICON_MD_LOCK);
			ImGui::PopItemWidth();
			ImGui::SameLine();

			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.2f, 0.35f, 0.9f, 1.0f });
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
			if (ImGui::Button("Z", ImGuiH::GetLineItemSize())) m_Scale.z = 0.0f;
			ImGui::PopStyleColor(3);
			ImGui::SameLine();
			ImGui::DragFloat("##Z", &m_Scale.z, 0.1f, 0.0f, 0.0f, "%.2f");
			ImGui::SameLine();
			ImGui::Button(ICON_MD_LOCK_OPEN);
			ImGui::PopItemWidth();

			ImGui::PopStyleVar();
			ImGui::Columns(1);
			ImGui::PopID();
		}
	}

	void TransformComponent::CalMatrix()
	{
		/**
		* @brief Use raidans
		*/
		glm::mat4 rotation = glm::toMat4(glm::quat(m_Rotation));
		m_ModelMatrix = glm::translate(glm::mat4(1.0f), m_Position) * rotation * glm::scale(glm::mat4(1.0f), m_Scale);
	}
}