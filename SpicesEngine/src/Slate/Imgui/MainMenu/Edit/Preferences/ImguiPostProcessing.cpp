/**
* @file ImguiPostProcessing.cpp.
* @brief The ImguiPostProcessing Class Implementation.
* @author Spices.
*/

#include "Pchheader.h"
#include "ImguiPostProcessing.h"
#include "World/World/World.h"

namespace Spices {

    ImguiPostProcessing::ImguiPostProcessing(
        const std::string& panelName ,
        FrameInfo&         frameInfo
    )
        : ImguiSlate(panelName, frameInfo)
    {
        SPICES_PROFILE_ZONE;
    }

    void ImguiPostProcessing::OnRender()
    {
        SPICES_PROFILE_ZONE;

        const float columeWidth = ImGuiH::GetLineItemSize().x * 10.0f;

        {
            SPICES_PROFILE_ZONEN("ImguiPostProcessing::Bloom");

            const auto material = ResourcePool<Material>::Load<Material>("PostProcessRenderer.Bloom.Default");
            
            ImGuiH::DrawMaterial("Bloom", columeWidth, material);
        }

        {
            SPICES_PROFILE_ZONEN("ImguiPostProcessing::ToneMapping");

            const auto material = ResourcePool<Material>::Load<Material>("PostProcessRenderer.ToneMapping.Default");

            ImGuiH::DrawTreeTitle("ToneMapping", nullptr, [&]() {

                material->GetConstantParams().for_each([&](const std::string& k, ConstantParams& v){
                    ImGuiH::DrawPropertyItem(k, columeWidth, nullptr, [&](){

                        if (k == "exposure")
                        {
                            if (ImGuiH::DrawMaterialConstParams<float>(material, ImGuiDataType_Float, 1, "%.3f", v))
                            {
                                FrameInfo::Get().m_World->Mark(World::NeedUpdateTLAS | World::FrushStableFrame);
                            }
                        }
                        if (k == "toneMapper")
                        {
                            int selected = std::any_cast<int>(v.value.paramValue);

                            static const char* toneMappers[] = { "AMD Tonemapper", "DX11DSK", "Reinhard", "Uncharted2Tonemap", "ACES", "No tonemapper" };
                            ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x);
                            if (ImGui::Combo("##", &selected, toneMappers, _countof(toneMappers)))
                            {
                                v.value.paramValue = selected;
                                material->UpdateMaterial();
                                FrameInfo::Get().m_World->Mark(World::NeedUpdateTLAS | World::FrushStableFrame);
                            }
                            ImGui::PopItemWidth();
                        }
                    });
                    return false;
                });
            });
        }
    }
}