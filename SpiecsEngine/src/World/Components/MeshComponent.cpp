/**
* @file MeshComponent.cpp.
* @brief The MeshComponent Class Implementation.
* @author Spiecs.
*/

#include "Pchheader.h"
#include "MeshComponent.h"

#include "imgui_internal.h"
#include "Render/FrameInfo.h"
#include "World/World/World.h"

namespace Spiecs {

	void MeshComponent::OnSerialize()
	{

	}

	void MeshComponent::OnDeSerialize()
	{

	}

	void MeshComponent::DrawThis()
	{
		SPIECS_PROFILE_ZONE;
		
		ImGui::Spacing();
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(2.0f, 6.0f));
		float columnWidth = ImGuiH::GetLineItemSize().x * 5.0f;

		ImGuiTreeNodeFlags treeNodeFlags = 
							ImGuiTreeNodeFlags_DefaultOpen      | 
							ImGuiTreeNodeFlags_AllowItemOverlap | 
							ImGuiTreeNodeFlags_Framed           | 
							ImGuiTreeNodeFlags_FramePadding     ;
							//ImGuiTreeNodeFlags_Bullet           ;
		
		{
			SPIECS_PROFILE_ZONEN("MeshComponent Mesh Pack");
			
			ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(0.196f, 0.204f, 0.2f, 1.0f));
			ImGui::PushStyleColor(ImGuiCol_HeaderHovered, ImVec4(0.164f, 0.18f, 0.184f, 1.0f));
			ImGui::PushStyleColor(ImGuiCol_HeaderActive, ImVec4(0.164f, 0.18f, 0.184f, 1.0f));
			ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.196f, 0.204f, 0.2f, 1.0f));
			
			if(ImGui::TreeNodeEx("Geometries", treeNodeFlags))
			{
				for (uint64_t i = 0; i < m_Mesh->GetPacks().size(); i++)
				{
					{
						SPIECS_PROFILE_ZONEN("MeshComponent Mesh Pack Item");
						
						std::stringstream ss;
						ss << "MeshPack [" << i << "]";
						if(ImGui::TreeNodeEx(ss.str().c_str(), treeNodeFlags | ImGuiTreeNodeFlags_Bullet))
						{
							ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(2.0f, 2.0f));
							const float imageIconSize = 3.0f * ImGuiH::GetLineItemSize().x + 2.0f * ImGui::GetStyle().ItemSpacing.y;
							const float seperatorWidthS = ImGuiH::GetLineItemSize().x - 3.0f * ImGui::GetStyle().IndentSpacing;
							
							{
								SPIECS_PROFILE_ZONEN("MeshComponent Mesh Pack Icon");
								
								ImGui::Spacing();
								ImGui::PushID("Geometries");
								ImGui::Columns(2, 0, false);
			
								ImGui::SetColumnWidth(0, columnWidth);
								ImGui::ColorButton("##", ImVec4(1.0f, 0.0f, 0.0f, 0.0f), 0, ImVec2(imageIconSize, imageIconSize));
								ImGui::NextColumn();

								ImGui::BeginGroup();

								const ImVec2 textSize = ImVec2(2.0f * ImGuiH::GetLineItemSize().x, ImGuiH::GetLineItemSize().y);
							
								{
									SPIECS_PROFILE_ZONEN("MeshComponent Mesh Pack Prim");
									
									std::stringstream ss0;
									ss0 << "/World/MeshComponent/Mesh/" << ss.str();
									ImGui::CenteredText("Prim", textSize);
									ImGui::SameLine(2.0f * ImGuiH::GetLineItemSize().x);
									char buffer0[256] = {};
									ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x - ImGuiH::GetLineItemSize().x);
									ImGui::InputTextWithHint("##_0", ss0.str().c_str(), buffer0, sizeof(buffer0));
									ImGui::PopItemWidth();
								}
								
								{
									SPIECS_PROFILE_ZONEN("MeshComponent Mesh Pack Path");
									
									std::stringstream ss1;
									ss1 << "/World/MeshComponent/" << ss.str();
									char buffer1[256] = {};
									ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x - ImGuiH::GetLineItemSize().x);
									ImGui::InputTextWithHint("##_2", ss1.str().c_str(), buffer1, sizeof(buffer1));
									ImGui::PopItemWidth();
								}
								
								{
									SPIECS_PROFILE_ZONEN("MeshComponent Mesh Pack Type");
									
									std::string type =  m_Mesh->GetPacks()[static_cast<uint32_t>(i)]->GetPackType();
									ImGui::CenteredText("Type", textSize);
									ImGui::SameLine(2.0f * ImGuiH::GetLineItemSize().x);
									char buffer2[256] = {};
									ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x - ImGuiH::GetLineItemSize().x);
									ImGui::InputTextWithHint("##_1", type.c_str(), buffer2, sizeof(buffer2));
									ImGui::PopItemWidth();
								}
								ImGui::EndGroup();
						
								ImGui::Columns(1);
								ImGui::PopID();
								ImGui::Spacing();
								ImGui::Separator();
							}

							ImGui::Spacing();
							
							{
								SPIECS_PROFILE_ZONEN("MeshComponent Mesh Pack Shadows");
								
								ImGui::PushID("Geometries Shadows");
								ImGui::Columns(2, 0, false);
			
								ImGui::SetColumnWidth(0, columnWidth);
								ImGui::Text("Cast Shadows");
								ImGui::NextColumn();

								static bool isCastShadow = true;
								ImGuiH::Checkbox(&isCastShadow);
								ImGui::SameLine();
								ImGui::SeparatorText("##");
								ImGui::SameLine(ImGui::GetContentRegionAvail().x - seperatorWidthS);
								if(ImGuiH::DrawResetIcon(isCastShadow != true)) { isCastShadow = true; }
								
								ImGui::Columns(1);
								ImGui::PopID();
							}

							{
								SPIECS_PROFILE_ZONEN("MeshComponent Mesh Pack Visible");
								
								ImGui::PushID("Geometries Visible");
								ImGui::Columns(2, 0, false);
			
								ImGui::SetColumnWidth(0, columnWidth);
								ImGui::Text("Visible");
								ImGui::NextColumn();

								static bool isVisible = true;
								ImGuiH::Checkbox(&isVisible);
								ImGui::SameLine();
								ImGui::SeparatorText("##");
								ImGui::SameLine(ImGui::GetContentRegionAvail().x - seperatorWidthS);
								if(ImGuiH::DrawResetIcon(isVisible != true)) { isVisible = true; }
								
								ImGui::Columns(1);
								ImGui::PopID();
							}

							ImGui::Spacing();
							
							ImGui::PopStyleVar();
							ImGui::TreePop();
						}
					}

					
				}
				
				ImGui::TreePop();
			}
			
			ImGui::PopStyleColor(4);
		}
		
		{
			SPIECS_PROFILE_ZONEN("MeshComponent Materials");
			
			ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(0.196f, 0.204f, 0.2f, 1.0f));
			ImGui::PushStyleColor(ImGuiCol_HeaderHovered, ImVec4(0.164f, 0.18f, 0.184f, 1.0f));
			ImGui::PushStyleColor(ImGuiCol_HeaderActive, ImVec4(0.164f, 0.18f, 0.184f, 1.0f));
			ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.196f, 0.204f, 0.2f, 1.0f));
			
			if(ImGui::TreeNodeEx("Materials", treeNodeFlags))
			{
				for (uint64_t i = 0; i < m_Mesh->GetPacks().size(); i++)
				{
					{
						SPIECS_PROFILE_ZONEN("MeshComponent Materials Item");
						
						std::stringstream ss;
						ss << "Materials [" << i << "]";
						if(ImGui::TreeNodeEx(ss.str().c_str(), treeNodeFlags | ImGuiTreeNodeFlags_Bullet))
						{
							ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(2.0f, 2.0f));
							const float imageIconSize = 3.0f * ImGuiH::GetLineItemSize().x + 2.0f * ImGui::GetStyle().ItemSpacing.y;
							const float seperatorWidthS = ImGuiH::GetLineItemSize().x - 3.0f * ImGui::GetStyle().IndentSpacing;
							
							{
								SPIECS_PROFILE_ZONEN("MeshComponent Materials Icon");
								
								ImGui::Spacing();
								ImGui::PushID("Materials");
								ImGui::Columns(2, 0, false);
			
								ImGui::SetColumnWidth(0, columnWidth);
								ImGui::ColorButton("##", ImVec4(0.0f, 1.0f, 0.0f, 0.0f), 0, ImVec2(imageIconSize, imageIconSize));
								ImGui::NextColumn();

								ImGui::BeginGroup();

								const ImVec2 textSize = ImVec2(2.0f * ImGuiH::GetLineItemSize().x, ImGuiH::GetLineItemSize().y);
							
								{
									SPIECS_PROFILE_ZONEN("MeshComponent Materials Prim");
									
									std::stringstream ss0;
									ss0 << "/World/MeshComponent/Mesh/" << ss.str();
									ImGui::CenteredText("Prim", textSize);
									ImGui::SameLine(2.0f * ImGuiH::GetLineItemSize().x);
									char buffer0[256] = {};
									ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x - ImGuiH::GetLineItemSize().x);
									ImGui::InputTextWithHint("##_0", ss0.str().c_str(), buffer0, sizeof(buffer0));
									ImGui::PopItemWidth();
								}
								
								{
									SPIECS_PROFILE_ZONEN("MeshComponent Materials Path");
									
									char buffer1[256] = {};
									ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x - ImGuiH::GetLineItemSize().x);
									ImGui::InputTextWithHint("##_2", m_Mesh->GetPacks()[i]->GetMaterial()->GetName().c_str(), buffer1, sizeof(buffer1));
									ImGui::PopItemWidth();
								}
								
								{
									SPIECS_PROFILE_ZONEN("MeshComponent Materials Type");
									
									ImGui::CenteredText("Type", textSize);
									ImGui::SameLine(2.0f * ImGuiH::GetLineItemSize().x);
									char buffer2[256] = {};
									ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x - ImGuiH::GetLineItemSize().x);
									ImGui::InputTextWithHint("##_1", "PathTracing Material", buffer2, sizeof(buffer2));
									ImGui::PopItemWidth();
								}
								ImGui::EndGroup();
						
								ImGui::Columns(1);
								ImGui::PopID();
								ImGui::Spacing();
							}
							
							ImGui::PopStyleVar();
							ImGui::TreePop();
						}
					}
				}
				
				ImGui::TreePop();
			}
			
			ImGui::PopStyleColor(4);
		}
		
		ImGui::PopStyleVar();
		ImGui::Spacing();
	}

	void MeshComponent::SetMesh(std::shared_ptr<Mesh> mesh)
	{
		SPIECS_PROFILE_ZONE;

		/**
		* @brief Set m_Mesh.
		*/
		m_Mesh = mesh;

		/**
		* @brief Mark World with MeshAddedToWorld bits.
		*/
		FrameInfo::Get().m_World->Mark(WorldMarkBits::MeshAddedToWorld);
	}
}