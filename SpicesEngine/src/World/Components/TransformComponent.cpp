/**
* @file TransformComponent.cpp.
* @brief The TransformComponent Class Implementation.
* @author Spices.
*/

#include "Pchheader.h"
#include "TransformComponent.h"

#include "Render/Vulkan/VulkanBuffer.h"
#include "Render/Vulkan/VulkanRenderBackend.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <imgui_internal.h>

#include "Render/FrameInfo.h"
#include "World/World/World.h"

namespace Spices {

	TransformComponent::TransformComponent()
		: Component()
	{
		SPICES_PROFILE_ZONE;

		m_ModelBuffer = std::make_shared<VulkanBuffer>(
			VulkanRenderBackend::GetState(),
			sizeof(glm::mat4),
			VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT ,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT   |
			VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
		);
	}

	void TransformComponent::OnSerialize()
	{

	}

	void TransformComponent::OnDeSerialize()
	{

	}

	void TransformComponent::DrawThis()
	{
		SPICES_PROFILE_ZONE;

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
			SPICES_PROFILE_ZONEN("TransformComponent Translate");

			ImGui::PushID("TransformComponent Translate");

			ImGui::Columns(2, 0, false);
			ImGui::SetColumnWidth(0, columeWidth);
			
			{
				SPICES_PROFILE_ZONEN("TransformComponent Translate table");

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
			}

			ImGui::NextColumn();

			{
				SPICES_PROFILE_ZONEN("TransformComponent Translate X");
				ImGui::PushID("TransformComponent Translate X");

				ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
				ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f, 0.2f, 0.2f, 1.0f });
				ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
				ImGui::Button("X", ImGuiH::GetLineItemSize());
				ImGui::PopStyleColor(3);
				ImGui::SameLine();
				ImGui::PushItemWidth(itemWidth);
				if(ImGui::DragFloat("##X", &m_Position.x, 0.1f, 0.0f, 0.0f, "%.2f"))
				{
					FrameInfo::Get().m_World->Mark(World::FrushStableFrame | World::NeedUpdateTLAS);
				}
				ImGui::PopItemWidth();
				ImGui::SameLine();
				if (ImGuiH::DrawResetIcon(m_Position.x != 0.0f)) 
				{ 
					m_Position.x = 0.0f; 
					FrameInfo::Get().m_World->Mark(World::FrushStableFrame | World::NeedUpdateTLAS);
				}
				ImGui::SameLine();
				ImGui::PopID();
			}

			{
				SPICES_PROFILE_ZONEN("TransformComponent Translate Y");
				ImGui::PushID("TransformComponent Translate Y");

				ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
				ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.3f, 0.8f, 0.3f, 1.0f });
				ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
				ImGui::Button("Y", ImGuiH::GetLineItemSize());
				ImGui::PopStyleColor(3);
				ImGui::SameLine();
				ImGui::PushItemWidth(itemWidth);
				if(ImGui::DragFloat("##Y", &m_Position.y, 0.1f, 0.0f, 0.0f, "%.2f"))
				{
					FrameInfo::Get().m_World->Mark(World::FrushStableFrame | World::NeedUpdateTLAS);
				}
				ImGui::PopItemWidth();
				ImGui::SameLine();
				if (ImGuiH::DrawResetIcon(m_Position.y != 0.0f)) 
				{
					m_Position.y = 0.0f; 
					FrameInfo::Get().m_World->Mark(World::FrushStableFrame | World::NeedUpdateTLAS);
				}
				ImGui::SameLine();
				ImGui::PopID();
			}

			{
				SPICES_PROFILE_ZONEN("TransformComponent Translate Z");
				ImGui::PushID("TransformComponent Translate Z");

				ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
				ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.2f, 0.35f, 0.9f, 1.0f });
				ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
				ImGui::Button("Z", ImGuiH::GetLineItemSize());
				ImGui::PopStyleColor(3);
				ImGui::SameLine();
				ImGui::PushItemWidth(itemWidth);
				if(ImGui::DragFloat("##Z", &m_Position.z, 0.1f, 0.0f, 0.0f, "%.2f"))
				{
					FrameInfo::Get().m_World->Mark(World::FrushStableFrame | World::NeedUpdateTLAS);
				}
				ImGui::PopItemWidth();
				ImGui::SameLine();
				if (ImGuiH::DrawResetIcon(m_Position.z != 0.0f)) 
				{ 
					m_Position.z = 0.0f; 
					FrameInfo::Get().m_World->Mark(World::FrushStableFrame | World::NeedUpdateTLAS);
				}
				ImGui::PopID();
			}

			ImGui::Columns(1);
			ImGui::PopID();
		}

		{
			SPICES_PROFILE_ZONEN("TransformComponent Rotate");

			ImGui::PushID("TransformComponent Rotate");

			ImGui::Columns(2, 0, false);
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
			
			{
				SPICES_PROFILE_ZONEN("TransformComponent Rotate table");

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
			}

			ImGui::NextColumn();

			{
				SPICES_PROFILE_ZONEN("TransformComponent Rotate X");
				ImGui::PushID("TransformComponent Rotate X");

				ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
				ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f, 0.2f, 0.2f, 1.0f });
				ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
				ImGui::Button("X", ImGuiH::GetLineItemSize());
				ImGui::PopStyleColor(3);
				ImGui::SameLine();
				ImGui::PushItemWidth(itemWidth);
				if(ImGui::DragFloat("##X", &m_Rotation.x, 0.1f, 0.0f, 0.0f, "%.2f"))
				{
					FrameInfo::Get().m_World->Mark(World::FrushStableFrame | World::NeedUpdateTLAS);
				}
				ImGui::PopItemWidth();
				ImGui::SameLine();
				if (ImGuiH::DrawResetIcon(m_Rotation.x != 0.0f)) 
				{ 
					m_Rotation.x = 0.0f; 
					FrameInfo::Get().m_World->Mark(World::FrushStableFrame | World::NeedUpdateTLAS);
				}
				ImGui::SameLine();
				ImGui::PopID();
			}

			{
				SPICES_PROFILE_ZONEN("TransformComponent Rotate Y");
				ImGui::PushID("TransformComponent Rotate Y");

				ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
				ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.3f, 0.8f, 0.3f, 1.0f });
				ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
				ImGui::Button("Y", ImGuiH::GetLineItemSize());
				ImGui::PopStyleColor(3);
				ImGui::SameLine();
				ImGui::PushItemWidth(itemWidth);
				if(ImGui::DragFloat("##Y", &m_Rotation.y, 0.1f, 0.0f, 0.0f, "%.2f"))
				{
					FrameInfo::Get().m_World->Mark(World::FrushStableFrame | World::NeedUpdateTLAS);
				}
				ImGui::PopItemWidth();
				ImGui::SameLine();
				if (ImGuiH::DrawResetIcon(m_Rotation.y != 0.0f)) 
				{ 
					m_Rotation.y = 0.0f; 
					FrameInfo::Get().m_World->Mark(World::FrushStableFrame | World::NeedUpdateTLAS);
				};
				ImGui::SameLine();
				ImGui::PopID();
			}

			{
				SPICES_PROFILE_ZONEN("TransformComponent Rotate Z");
				ImGui::PushID("TransformComponent Rotate Z");

				ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
				ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.2f, 0.35f, 0.9f, 1.0f });
				ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
				ImGui::Button("Z", ImGuiH::GetLineItemSize());
				ImGui::PopStyleColor(3);
				ImGui::SameLine();
				ImGui::PushItemWidth(itemWidth);
				if(ImGui::DragFloat("##Z", &m_Rotation.z, 0.1f, 0.0f, 0.0f, "%.2f"))
				{
					FrameInfo::Get().m_World->Mark(World::FrushStableFrame | World::NeedUpdateTLAS);
				}
				ImGui::PopItemWidth();
				ImGui::SameLine();
				if (ImGuiH::DrawResetIcon(m_Rotation.z != 0.0f)) 
				{ 
					m_Rotation.z = 0.0f;
					FrameInfo::Get().m_World->Mark(World::FrushStableFrame | World::NeedUpdateTLAS);
				}
				ImGui::PopID();
			}

			ImGui::Columns(1);
			ImGui::PopID();
		}

		{
			SPICES_PROFILE_ZONEN("TransformComponent Scale");

			ImGui::PushID("TransformComponent Scale");

			ImGui::Columns(2, 0, false);
			ImGui::SetColumnWidth(0, columeWidth);

			{
				SPICES_PROFILE_ZONEN("TransformComponent Scale table");

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
			}

			ImGui::NextColumn();

			{
				SPICES_PROFILE_ZONEN("TransformComponent Scale X");
				ImGui::PushID("TransformComponent Scale X");

				ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
				ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f, 0.2f, 0.2f, 1.0f });
				ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
				ImGui::Button("X", ImGuiH::GetLineItemSize());
				ImGui::PopStyleColor(3);
				ImGui::SameLine();
				ImGui::PushItemWidth(itemWidth);
				if(ImGui::DragFloat("##X", &m_Scale.x, 0.1f, 0.0f, 0.0f, "%.2f"))
				{
					FrameInfo::Get().m_World->Mark(World::FrushStableFrame | World::NeedUpdateTLAS);
				}
				ImGui::PopItemWidth();
				ImGui::SameLine();
				if (ImGuiH::DrawResetIcon(m_Scale.x != 1.0f)) 
				{ 
					m_Scale.x = 1.0f; 
					FrameInfo::Get().m_World->Mark(World::FrushStableFrame | World::NeedUpdateTLAS);
				}
				ImGui::SameLine();
				ImGui::PopID();
			}

			{
				SPICES_PROFILE_ZONEN("TransformComponent Scale Y");
				ImGui::PushID("TransformComponent Scale Y");

				ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
				ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.3f, 0.8f, 0.3f, 1.0f });
				ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
				ImGui::Button("Y", ImGuiH::GetLineItemSize());
				ImGui::PopStyleColor(3);
				ImGui::SameLine();
				ImGui::PushItemWidth(itemWidth);
				if(ImGui::DragFloat("##Y", &m_Scale.y, 0.1f, 0.0f, 0.0f, "%.2f"))
				{
					FrameInfo::Get().m_World->Mark(World::FrushStableFrame | World::NeedUpdateTLAS);
				}
				ImGui::PopItemWidth();
				ImGui::SameLine();
				if (ImGuiH::DrawResetIcon(m_Scale.y != 1.0f)) 
				{ 
					m_Scale.y = 1.0f; 
					FrameInfo::Get().m_World->Mark(World::FrushStableFrame | World::NeedUpdateTLAS);
				}
				ImGui::SameLine();
				ImGui::PopID();
			}

			{
				SPICES_PROFILE_ZONEN("TransformComponent Scale Z");
				ImGui::PushID("TransformComponent Scale Z");

				ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
				ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.2f, 0.35f, 0.9f, 1.0f });
				ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
				ImGui::Button("Z", ImGuiH::GetLineItemSize());
				ImGui::PopStyleColor(3);
				ImGui::SameLine();
				ImGui::PushItemWidth(itemWidth);
				if(ImGui::DragFloat("##Z", &m_Scale.z, 0.1f, 0.0f, 0.0f, "%.2f"))
				{
					FrameInfo::Get().m_World->Mark(World::FrushStableFrame | World::NeedUpdateTLAS);
				}
				ImGui::PopItemWidth();
				ImGui::SameLine();
				if (ImGuiH::DrawResetIcon(m_Scale.z != 1.0f))
				{ 
					m_Scale.z = 1.0f; 
					FrameInfo::Get().m_World->Mark(World::FrushStableFrame | World::NeedUpdateTLAS);
				}
				ImGui::PopID();
			}

			ImGui::Columns(1);
			ImGui::PopID();
		}
		
		ImGui::PopStyleVar();
		ImGui::Spacing();
	}

	glm::mat4 TransformComponent::GetRotateMatrix() const
	{
		SPICES_PROFILE_ZONE;

		/**
		* @brief Use raidans
		*/
		return glm::toMat4(glm::quat({glm::radians(m_Rotation.x), glm::radians(m_Rotation.y), glm::radians(m_Rotation.z)}));
	}

	void TransformComponent::ClearMarkerWithBits(TransformComponentFlags flags)
	{
		SPICES_PROFILE_ZONE;

		if (m_Marker & flags)
		{
			m_Marker ^= flags;
		}
	}

	uint64_t TransformComponent::GetModelBufferAddress() const
	{
		SPICES_PROFILE_ZONE;

		return m_ModelBuffer->GetAddress();
	}

	void TransformComponent::CalMatrix()
	{
		SPICES_PROFILE_ZONE;

		const glm::mat4 rotation = GetRotateMatrix();
		m_ModelMatrix = glm::translate(glm::mat4(1.0f), m_Position) * rotation * glm::scale(glm::mat4(1.0f), m_Scale);

		m_ModelBuffer->WriteToBuffer(glm::value_ptr(m_ModelMatrix));
	}
}
