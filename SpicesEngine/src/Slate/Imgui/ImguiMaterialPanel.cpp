/**
* @file ImguiMaterialPanel.cpp.
* @brief The ImguiMaterialPanel Class Implementation.
* @author Spices.
*/

#include "Pchheader.h"
#include "ImguiMaterialPanel.h"

namespace Spices {

    void ImguiMaterialPanel::OnRender()
    {
        SPICES_PROFILE_ZONE;

		/**
		* @brief Begin render Property.
		*/
		Begin();

        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(2.0f, 4.0f));

        /**
        * @brief Begin render Search Input Text.
        */
        {
            SPICES_PROFILE_ZONEN("ImguiMaterialPanel::Search");

            ImGui::Spacing();
            ImGui::PushItemWidth(m_PanelSize.x);
            char search[128] = "";
            if (ImGui::InputTextWithHint("##", ICON_TEXT(ICON_MD_SEARCH, Search), search, 128)) {}
            ImGui::PopItemWidth();
        }
        
        if(m_FrameInfo.m_PickEntityID.size() == 0)
        {
            ImGui::PopStyleVar();
            End();
            return;
        }
        
        Entity entity(static_cast<entt::entity>(*m_FrameInfo.m_PickEntityID.end_k()), m_FrameInfo.m_World.get());
        if(!entity.HasComponent<MeshComponent>())
        {
            ImGui::PopStyleVar();
            End();
            return;
        }

        MeshComponent meshComp = entity.GetComponent<MeshComponent>();
        const uint32_t materialIndex = FrameInfo::Get().m_PickMaterial;
                    
        if(!meshComp.GetMesh()->GetPacks().has_key(materialIndex))
        {
            ImGui::PopStyleVar();
            End();
            return;
        }

        float columeWidth = ImGuiH::GetLineItemSize().x * 6.5f;
    	ImGuiTreeNodeFlags treeNodeFlags = 
									ImGuiTreeNodeFlags_DefaultOpen      | 
									ImGuiTreeNodeFlags_AllowItemOverlap | 
									ImGuiTreeNodeFlags_Framed           | 
									ImGuiTreeNodeFlags_FramePadding     ;
    							  //ImGuiTreeNodeFlags_Bullet           ;
    	
        const std::shared_ptr<MeshPack>& meshPack = *meshComp.GetMesh()->GetPacks().find_value(materialIndex);
        std::shared_ptr<Material> material = meshPack->GetMaterial();

        /**
        * @brief End if not a select entity.
        */
        if (m_FrameInfo.m_PickEntityID.size() == 0)
        {
            SPICES_PROFILE_ZONEN("ImguiMaterialPanel::End");
            
            ImGui::PopStyleVar();
            End();

            return;
        }

        /**
        * @brief Render Datails.
        */
        {
            SPICES_PROFILE_ZONEN("ImguiMaterialPanel::Datails");

            {
                ImGui::PushID("ImguiMaterialPanel::Add");

                ImGui::Columns(2, 0, false);
                ImGui::SetColumnWidth(0, ImGuiH::GetLineItemSize().x * 4.0f);
                ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.16f, 0.16f, 0.16f, 1.0f));
                ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.619f, 0.619f, 0.619f, 1.0f));
                ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.408f, 0.451f, 0.18f, 1.0f));
                ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.341f, 0.486f, 0.353f, 1.0f));
                ImGui::Button(ICON_TEXT(ICON_MD_ADD, Add));
                ImGui::PopStyleColor(4);
                ImGui::NextColumn();
                
                ImGui::PushItemWidth(m_PanelSize.x - ImGuiH::GetLineItemSize().x * 4.0f);
                char text[128] = "";
                if (ImGui::InputTextWithHint("##", material->GetName().c_str(), text, 128)) {}
                ImGui::PopItemWidth();

                ImGui::Columns(1);
                ImGui::PopID();
            }

            {
                ImGui::PushID("ImguiMaterialPanel::Prim Path");
                ImGui::Columns(2, 0, false);
                ImGui::SetColumnWidth(0, ImGuiH::GetLineItemSize().x * 4.0f);
                ImGui::Text("Prim Path");
                ImGui::NextColumn();

                std::stringstream ss;
                ss << "/" << material->GetName();
                ImGui::PushItemWidth(m_PanelSize.x - ImGuiH::GetLineItemSize().x * 4.0f);
                char textpath[128] = "";
                if (ImGui::InputTextWithHint("##", ss.str().c_str(), textpath, 128)) {}
                ImGui::PopItemWidth();

                ImGui::Columns(1);
                ImGui::PopID();
            }

            {
                ImGui::PushID("ImguiMaterialPanel::Instanceable");
                ImGui::Columns(2, 0, false);
                ImGui::SetColumnWidth(0, ImGuiH::GetLineItemSize().x * 4.0f);
                ImGui::Text("Instanceable");
                ImGui::NextColumn();

                static bool isChecked;
                ImGuiH::Checkbox(&isChecked);
                ImGui::Columns(1);
                ImGui::PopID();
            }
        }


        /**
        * @brief Render All Parameter.
        */
        {
            SPICES_PROFILE_ZONEN("ImguiMaterialPanel::Parameter");
            
            {
                SPICES_PROFILE_ZONEN("ImguiMaterialPanel::ShaderParameter");

                DrawParameter("Shader", [&](){
                    for(auto& pair : material->GetShaderPath())
                    {
						ImGuiH::DrawPropertyItem(pair.first, columeWidth, [&](){

						    ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x - ImGuiH::GetLineItemSize().x);
						    char buffer[128] = {};
						    ImGui::InputTextWithHint("##", pair.second[0].c_str(), buffer, sizeof(buffer));
						    ImGui::PopItemWidth();

						    ImGui::SameLine();
						    ImGuiH::DrawResetIcon(false);
						});
                    }
                });
            }

            {
                SPICES_PROFILE_ZONEN("ImguiMaterialPanel::TextureParameter");

                DrawParameter("Texture", [&](){
                    material->GetTextureParams().for_each([&](const auto& k, const auto& v) {
  
                        ImGuiH::DrawPropertyItem(k, columeWidth, [&](){

                            ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x - ImGuiH::GetLineItemSize().x);
                            char buffer[128] = {};
                            ImGui::InputTextWithHint("##", v.texturePath.c_str(), buffer, sizeof(buffer));
                            ImGui::PopItemWidth();

                            ImGui::SameLine();
                            ImGuiH::DrawResetIcon(false);
                        });
                        return false;
                    });
                });
            }

            {
                SPICES_PROFILE_ZONEN("ImguiMaterialPanel::ConstantParameter");

                DrawParameter("Parameter", [&](){
                    material->GetConstantParams().for_each([&](const std::string& k, ConstantParam& v){
                        ImGuiH::DrawPropertyItem(k, columeWidth, [&](){
                            if(v.paramType == "float")
                            {
                                float f = std::any_cast<float>(v.paramValue);
                                ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x - ImGuiH::GetLineItemSize().x);
                                if(ImGui::DragFloat("##", &f, 0.01f, 0.0f, 0.0f, "%.2f"))
                                {
                                    v.paramValue = f;
                                    material->UpdateMaterial();
                                    FrameInfo::Get().m_World->Mark(World::NeedUpdateTLAS | World::FrushStableFrame);
                                }
                                ImGui::PopItemWidth();
                            }
                            else if(v.paramType == "float2")
                            {
                                const glm::vec2 value = std::any_cast<glm::vec2>(v.paramValue);
                                float f[2] = { value.x, value.y };
                                ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x - ImGuiH::GetLineItemSize().x);
                                if(ImGui::DragFloat2("##", f, 0.01f, 0.0f, 0.0f, "%.2f"))
                                {
                                    v.paramValue = glm::vec2(f[0], f[1]);
                                    material->UpdateMaterial();
                                    FrameInfo::Get().m_World->Mark(World::NeedUpdateTLAS | World::FrushStableFrame);
                                }
                                ImGui::PopItemWidth();
                            }
                            else if(v.paramType == "float3")
                            {
                                const glm::vec3 value = std::any_cast<glm::vec3>(v.paramValue);
                                float f[3] = { value.x, value.y , value.z };
                                ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x - ImGuiH::GetLineItemSize().x);
                                if(ImGui::DragFloat3("##", f, 0.01f, 0.0f, 0.0f, "%.2f"))
                                {
                                    v.paramValue = glm::vec3(f[0], f[1], f[2]);
                                    material->UpdateMaterial();
                                    FrameInfo::Get().m_World->Mark(World::NeedUpdateTLAS | World::FrushStableFrame);
                                }
                                ImGui::PopItemWidth();
                            }
                            else if(v.paramType == "float4")
                            {
                                const glm::vec4 value = std::any_cast<glm::vec4>(v.paramValue);
                                float f[4] = { value.x, value.y, value.z, value.w };
                                ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x - ImGuiH::GetLineItemSize().x);
                                if(ImGui::DragFloat4("##", f, 0.01f, 0.0f, 0.0f, "%.2f"))
                                {
                                    v.paramValue = glm::vec4(f[0], f[1], f[2], f[3]);
                                    material->UpdateMaterial();
                                    FrameInfo::Get().m_World->Mark(World::NeedUpdateTLAS | World::FrushStableFrame);
                                }
                                ImGui::PopItemWidth();
                            }
                            else if(v.paramType == "int")
                            {
                                int value = std::any_cast<int>(v.paramValue);
                                ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x - ImGuiH::GetLineItemSize().x);
                                if(ImGui::DragInt("##", &value, 0.2f))
                                {
                                    v.paramValue = value;
                                    material->UpdateMaterial();
                                    FrameInfo::Get().m_World->Mark(World::NeedUpdateTLAS | World::FrushStableFrame);
                                }
                                ImGui::PopItemWidth();
                            }
                            ImGui::SameLine();
                            ImGuiH::DrawResetIcon(false);
                        });
                        return false;
                    });
                });
            }
        }
        
		/**
		* @brief End render Property.
		*/
        ImGui::PopStyleVar();
		End();
	}
}