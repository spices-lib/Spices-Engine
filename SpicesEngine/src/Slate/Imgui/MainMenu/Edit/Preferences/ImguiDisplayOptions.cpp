/**
* @file ImguiDisplayOptions.cpp.
* @brief The ImguiDisplayOptions Class Implementation.
* @author Spices.
*/

#include "Pchheader.h"
#include "ImguiDisplayOptions.h"
#include "World/World/World.h"

namespace Spices {

    ImguiDisplayOptions::ImguiDisplayOptions(
        const std::string& panelName ,
        FrameInfo&         frameInfo
    )
        : ImguiSlate(panelName, frameInfo)
    {
        SPICES_PROFILE_ZONE;
    }

    void ImguiDisplayOptions::OnRender()
    {
        SPICES_PROFILE_ZONE;
        
        {
            SPICES_PROFILE_ZONEN("ImguiDisplayOptions::Grid");

            auto material = ResourcePool<Material>::Load<Material>("ViewportGridRenderer.ViewportGrid.Default");
            const float columeWidth = ImGuiH::GetLineItemSize().x * 10.0f;

            ImGuiH::DrawTreeTitle("Grid", nullptr, [&]() { 

                material->GetConstantParams().for_each([&](const std::string& k, ConstantParams& v){
                    ImGuiH::DrawPropertyItem(k, columeWidth, nullptr, [&](){
                        if(v.value.paramType == "float")
                        {
                            if (ImGuiH::DrawMaterialConstParams<float>(material, ImGuiDataType_Float, 1, "%.3f", v))
                            {
                                FrameInfo::Get().m_World->Mark(World::NeedUpdateTLAS | World::FrushStableFrame);
                            }
                        }
                        else if(v.value.paramType == "float2")
                        {
                            if (ImGuiH::DrawMaterialConstParams<glm::vec2>(material, ImGuiDataType_Float, 2, "%.3f", v))
                            {
                                FrameInfo::Get().m_World->Mark(World::NeedUpdateTLAS | World::FrushStableFrame);
                            }
                        }
                        else if(v.value.paramType == "float3")
                        {
                            if (ImGuiH::DrawMaterialConstParams<glm::vec3>(material, ImGuiDataType_Float, 3, "%.3f", v))
                            {
                                FrameInfo::Get().m_World->Mark(World::NeedUpdateTLAS | World::FrushStableFrame);
                            }
                        }
                        else if(v.value.paramType == "float4")
                        {
                            if (ImGuiH::DrawMaterialConstParams<glm::vec4>(material, ImGuiDataType_Float, 4, "%.3f", v))
                            {
                                FrameInfo::Get().m_World->Mark(World::NeedUpdateTLAS | World::FrushStableFrame);
                            }
                        }
                        else if(v.value.paramType == "int")
                        {
                            if (ImGuiH::DrawMaterialConstParams<int>(material, ImGuiDataType_S32, 1, "%d", v))
                            {
                                FrameInfo::Get().m_World->Mark(World::NeedUpdateTLAS | World::FrushStableFrame);
                            }
                        }
                        else if (v.value.paramType == "bool")
                        {
                            bool  f = std::any_cast<bool>(v.value.paramValue);
                            bool nf = f;
                            bool df = std::any_cast<bool>(v.defaultValue.paramValue);

                            const float seperatorWidthS = ImGuiH::GetLineItemSize().x - 3.0f * ImGui::GetStyle().IndentSpacing;
                            ImGuiH::Checkbox(&nf);
                            if (nf != f)
                            {
                                v.value.paramValue = nf;
                                material->UpdateMaterial();
                            }
                            ImGui::SameLine();
                            ImGui::PushItemWidth(100.0f);
                            ImGui::SeparatorText("##");
                            ImGui::PopItemWidth();
                            ImGui::SameLine(ImGui::GetContentRegionAvail().x - ImGui::GetStyle().ItemSpacing.x);
                            if (ImGuiH::DrawResetIcon(f != df))
                            {
                                v.value.paramValue = df;
                                material->UpdateMaterial();
                            }
                        }
                    });
                    return false;
                });
            });
        }
    }
}