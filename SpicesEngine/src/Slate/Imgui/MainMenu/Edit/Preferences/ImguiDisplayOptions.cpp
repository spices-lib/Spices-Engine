/**
* @file ImguiDisplayOptions.cpp.
* @brief The ImguiDisplayOptions Class Implementation.
* @author Spices.
*/

#include "Pchheader.h"
#include "ImguiDisplayOptions.h"

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
                    ImGuiH::DrawPropertyItem(k, columeWidth, [&](){
                        if(v.value.paramType == "float")
                        {
                            ImGuiH::DrawMaterialConstParams<float>(material, ImGuiDataType_Float, 1, k, v);
                        }
                        else if(v.value.paramType == "float2")
                        {
                            ImGuiH::DrawMaterialConstParams<glm::vec2>(material, ImGuiDataType_Float, 2, k, v);
                        }
                        else if(v.value.paramType == "float3")
                        {
                            ImGuiH::DrawMaterialConstParams<glm::vec3>(material, ImGuiDataType_Float, 3, k, v);
                        }
                        else if(v.value.paramType == "float4")
                        {
                            ImGuiH::DrawMaterialConstParams<glm::vec4>(material, ImGuiDataType_Float, 4, k, v);
                        }
                        else if(v.value.paramType == "int")
                        {
                            ImGuiH::DrawMaterialConstParams<int>(material, ImGuiDataType_S32, 1, k, v);
                        }
                    });
                    return false;
                });
            });
        }
    }
}