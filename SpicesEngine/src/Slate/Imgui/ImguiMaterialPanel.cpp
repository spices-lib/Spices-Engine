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
        * @brief Search String.
        */
        static std::string searchString;
        static bool isEnableSearch = false;

        /**
        * @brief Begin render Search Input Text.
        */
        {
            SPICES_PROFILE_ZONEN("ImguiMaterialPanel::Search");

            ImGui::Spacing();
            ImGui::PushItemWidth(m_PanelSize.x - ImGuiH::GetLineItemSize().x * 2.0f - ImGui::GetStyle().WindowPadding.x);
            static char search[256] = {};
            if (ImGui::InputTextWithHint("##", ICON_TEXT(ICON_MD_SEARCH, Search), search, 128))
            {
                searchString = std::string(search);
                if (searchString.size() == 0) isEnableSearch = false;
                else isEnableSearch = true;
            }
            ImGui::PopItemWidth();

            ImGui::SameLine(m_PanelSize.x - ImGuiH::GetLineItemSize().x * 2.0f);
            ImGui::Button(ICON_MD_FILTER_ALT, ImGuiH::GetLineItemSize());
            ImGui::SameLine(m_PanelSize.x - ImGuiH::GetLineItemSize().x * 1.0f);
            ImGui::Button(ICON_MD_REORDER, ImGuiH::GetLineItemSize());
            ImGui::Spacing();
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
                if (ImGui::InputTextWithHint("##", ICON_TEXT_ROW(" ", material->GetName().c_str()), text, 128)) {}
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
                if (ImGui::InputTextWithHint("##", ICON_TEXT_ROW(" ", ss.str().c_str()), textpath, 128)) {}
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

                ImGuiH::DrawTreeTitle("Shader", nullptr, [&](){
                    for(auto& pair : material->GetShaderPath())
                    {
						ImGuiH::DrawPropertyItem(pair.first, columeWidth, nullptr, [&](){

						    ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x - ImGuiH::GetLineItemSize().x);
						    char buffer[128] = {};
						    ImGui::InputTextWithHint("##", ICON_TEXT_ROW(" ", pair.second[0].c_str()), buffer, sizeof(buffer));
						    ImGui::PopItemWidth();

						    ImGui::SameLine();
						    ImGuiH::DrawResetIcon(false);
						});
                    }
                });
            }

            {
                SPICES_PROFILE_ZONEN("ImguiMaterialPanel::TextureParameter");

                ImGuiH::DrawTreeTitle("Texture", nullptr, [&](){
                    material->GetTextureParams().for_each([&](const auto& k, const auto& v) {
  
                        ImGuiH::DrawPropertyItem(k, columeWidth, nullptr, [&](){

                            ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x - ImGuiH::GetLineItemSize().x);
                            char buffer[128] = {};
                            ImGui::InputTextWithHint("##", ICON_TEXT_ROW(" ", v.texturePath.c_str()), buffer, sizeof(buffer));
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

                ImGuiH::DrawTreeTitle("Parameter", nullptr, [&](){
                    material->GetConstantParams().for_each([&](const std::string& k, ConstantParams& v){
                        ImGuiH::DrawPropertyItem(k, columeWidth, nullptr, [&](){
                            if (v.value.paramType == "float")
                            {
                                ImGuiH::DrawMaterialConstParams<float>(material, ImGuiDataType_Float, 1, k, v);
                            }
                            else if (v.value.paramType == "float2")
                            {
                                ImGuiH::DrawMaterialConstParams<glm::vec2>(material, ImGuiDataType_Float, 2, k, v);
                            }
                            else if (v.value.paramType == "float3")
                            {
                                ImGuiH::DrawMaterialConstParams<glm::vec3>(material, ImGuiDataType_Float, 3, k, v);
                            }
                            else if (v.value.paramType == "float4")
                            {
                                ImGuiH::DrawMaterialConstParams<glm::vec4>(material, ImGuiDataType_Float, 4, k, v);
                            }
                            else if (v.value.paramType == "int")
                            {
                                ImGuiH::DrawMaterialConstParams<int>(material, ImGuiDataType_S32, 1, k, v);
                            }
                            else if (v.value.paramType == "bool")
                            {

                            }
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