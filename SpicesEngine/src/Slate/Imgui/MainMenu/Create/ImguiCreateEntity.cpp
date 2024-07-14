/**
* @file ImguiCreateEntity.cpp.
* @brief The ImguiCreateEntity Class Implementation.
* @author Spices.
*/

#include "Pchheader.h"
#include "ImguiCreateEntity.h"

#include "Systems/SlateSystem.h"

namespace Spices {

    ImguiCreateEntity::ImguiCreateEntity(
        const std::string& panelName , 
        FrameInfo&         frameInfo
    )
        : ImguiSlate(panelName, frameInfo)
    {}

    void ImguiCreateEntity::OnRender()
    {
        SPICES_PROFILE_ZONE;

        /**
        * @brief Begin render Create.
        */
        if (ImGui::BeginMenu("Create"))
        {
            ImGuiH::MainMenuTitleSeparator();

            /**
            * @brief Begin render Mesh.
            */
            {
                SPICES_PROFILE_ZONEN("Render Mesh");

                if (ImGui::BeginMenu(ICON_TEXT(ICON_MD_POLYMER, Mesh)))
                {
                    if (ImGui::MenuItem("Cone")) {}
                    if (ImGui::MenuItem("Cube")) {}
                    if (ImGui::MenuItem("Cylinder")) {}
                    if (ImGui::MenuItem("Disk")) {}
                    if (ImGui::MenuItem("Plane")) {}
                    if (ImGui::MenuItem("Sphere")) {}
                    if (ImGui::MenuItem("Torus")) {}
                    ImGui::Separator();
                    if (ImGui::MenuItem("Settings")) {}

                    ImGui::EndMenu();
                }
            }
            
            /**
            * @brief Begin render Shape.
            */
            {
                SPICES_PROFILE_ZONEN("Render Shape");

                if (ImGui::BeginMenu(ICON_TEXT(ICON_MD_FORMAT_SHAPES, Shape)))
                {
                    if (ImGui::MenuItem("Capsule")) {}
                    if (ImGui::MenuItem("Cone")) {}
                    if (ImGui::MenuItem("Cube")) {}
                    if (ImGui::MenuItem("Cylinder")) {}
                    if (ImGui::MenuItem("Sphere")) {}
                    ImGui::Separator();
                    bool isHighQuality = true;
                    if (ImGui::Checkbox("High Quality", &isHighQuality)) {}

                    ImGui::EndMenu();
                }
            }

            /**
            * @brief Begin render Light.
            */
            {
                SPICES_PROFILE_ZONEN("Render Light");

                if (ImGui::BeginMenu(ICON_TEXT(ICON_MD_WB_TWIGHLIGHT, Light)))
                {
                    if (ImGui::MenuItem("Cylinder Light")) {}
                    if (ImGui::MenuItem("Disk Light")) {}
                    if (ImGui::MenuItem("Distance Light")) {}
                    if (ImGui::MenuItem("Dome Light")) {}
                    if (ImGui::MenuItem("Rect Light")) {}
                    if (ImGui::MenuItem("Sphere Light")) {}

                    ImGui::EndMenu();
                }
            }

            /**
            * @brief Begin render Audio.
            */
            {
                SPICES_PROFILE_ZONEN("Render Audio");

                if (ImGui::BeginMenu(ICON_TEXT(ICON_MD_MULTITRACK_AUDIO, Audio)))
                {
                    if (ImGui::MenuItem("Spatial Sound")) {}
                    if (ImGui::MenuItem("Non-Spatial Sound")) {}
                    if (ImGui::MenuItem("Listener")) {}

                    ImGui::EndMenu();
                }
            }

            /**
            * @brief Begin render Camera.
            */
            if (ImGui::MenuItem(ICON_TEXT(ICON_MD_VIDEO_CAMERA_FRONT, Camera))) {}

            /**
            * @brief Begin render Scope.
            */
            if (ImGui::MenuItem(ICON_TEXT(ICON_MD_FOLDER, Scope))) {}

            /**
            * @brief Begin render Xform.
            */
            if (ImGui::MenuItem(ICON_TEXT(ICON_MD_LINE_AXIS, Xform))) {}
 
            /**
            * @brief Begin render Material.
            */
            {
                SPICES_PROFILE_ZONEN("Render Material");

                if (ImGui::BeginMenu(ICON_TEXT(ICON_MD_PANORAMA_PHOTOSPHERE_SELECT, Material)))
                {
                    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.5f, 0.5f, 0.5f, 1.0f));
                    ImGui::SeparatorText("Advanced");
                    ImGui::PopStyleColor();
                    if (ImGui::MenuItem("Surface")) {}
                    if (ImGui::MenuItem("SurfaceBlend")) {}
                    if (ImGui::MenuItem("SurfaceLite")) {}
                    if (ImGui::BeginMenu("SurfacePresets"))
                    {
                        if (ImGui::MenuItem("Surface IncandescentBulb")) {}
                        if (ImGui::MenuItem("Surface BrushMetal")) {}
                        if (ImGui::MenuItem("Surface Chrome")) {}
                        if (ImGui::MenuItem("Surface Copper")) {}
                        if (ImGui::MenuItem("Surface Gold")) {}
                        if (ImGui::MenuItem("Surface Foam")) {}
                        if (ImGui::MenuItem("Surface Rubber")) {}
                        if (ImGui::MenuItem("Surface CarPaint")) {}
                        if (ImGui::MenuItem("Surface CarPaintMetallic")) {}
                        if (ImGui::MenuItem("Surface GlossyPaint")) {}
                        if (ImGui::MenuItem("Surface TwoToneCarPaint")) {}
                        if (ImGui::MenuItem("Surface Default")) {}
                        if (ImGui::MenuItem("Surface PeanutButter")) {}
                        if (ImGui::MenuItem("Surface SkimMilk")) {}
                        if (ImGui::MenuItem("Surface WholeMilk")) {}
                        if (ImGui::MenuItem("Surface Ceramic")) {}
                        if (ImGui::MenuItem("Surface Clay")) {}
                        if (ImGui::MenuItem("Surface Plastic")) {}
                        if (ImGui::MenuItem("Surface Skin1")) {}
                        if (ImGui::MenuItem("Surface Skin2")) {}
                        if (ImGui::MenuItem("Surface Skin3")) {}
                        if (ImGui::MenuItem("Surface Skin4")) {}
                        if (ImGui::MenuItem("Surface Velvet")) {}
                        if (ImGui::MenuItem("Surface Honey")) {}
                        if (ImGui::MenuItem("Surface MapleSyrup")) {}
                        if (ImGui::MenuItem("Surface OrangeJuice")) {}
                        if (ImGui::MenuItem("Surface DustedGlass")) {}
                        if (ImGui::MenuItem("Surface FrostedGlass")) {}
                        if (ImGui::MenuItem("Surface Glass")) {}
                        if (ImGui::MenuItem("Surface Blood")) {}
                        if (ImGui::MenuItem("Surface Bubble")) {}
                        if (ImGui::MenuItem("Surface Wax")) {}
                        if (ImGui::MenuItem("Surface Polyethylene")) {}
                        if (ImGui::MenuItem("Surface Diamond")) {}
                        if (ImGui::MenuItem("Surface Jade")) {}
                        if (ImGui::MenuItem("Surface ClearWater")) {}
                        if (ImGui::MenuItem("Surface DeepWater")) {}

                        ImGui::EndMenu();
                    } 
                    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.5f, 0.5f, 0.5f, 1.0f));
                    ImGui::SeparatorText("Base");
                    ImGui::PopStyleColor();
                    if (ImGui::MenuItem("Glass")) {}
                    if (ImGui::MenuItem("PBR")) {}
                    if (ImGui::MenuItem("PBRBase")) {}
                    if (ImGui::MenuItem("PBR ClearCoat")) {}
                    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.5f, 0.5f, 0.5f, 1.0f));
                    ImGui::SeparatorText("Hair");
                    ImGui::PopStyleColor();
                    if (ImGui::MenuItem("Hair")) {}
                    if (ImGui::BeginMenu("HairPresets")) 
                    {
                        if (ImGui::MenuItem("Hair Blonde")) {}
                        if (ImGui::MenuItem("Hair Brown")) {}
                        if (ImGui::MenuItem("Hair Auburn")) {}
                        if (ImGui::MenuItem("Hair Black")) {}
                        if (ImGui::MenuItem("Hair Wet")) {}

                        ImGui::EndMenu();
                    }
                    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.5f, 0.5f, 0.5f, 1.0f));
                    ImGui::SeparatorText("SimReady");
                    ImGui::PopStyleColor();
                    if (ImGui::MenuItem("SimPBR")) {}
                    if (ImGui::MenuItem("SimPBR Translucent")) {}
                    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.5f, 0.5f, 0.5f, 1.0f));
                    ImGui::SeparatorText("Volumes");
                    ImGui::PopStyleColor();
                    if (ImGui::MenuItem("VolumeDensity")) {}
                    if (ImGui::BeginMenu("VolumeNoise"))
                    {
                        if (ImGui::MenuItem("VolumeWorleyNoise")) {}
                        if (ImGui::MenuItem("VolumePerlinNoise")) {}

                        ImGui::EndMenu();
                    }
                    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.5f, 0.5f, 0.5f, 1.0f));
                    ImGui::SeparatorText("Usd Materials");
                    ImGui::PopStyleColor();
                    if (ImGui::MenuItem("USD Preview Surface")) {}
                    if (ImGui::MenuItem("USD Preview Surface Texture")) {}
                    if (ImGui::MenuItem("Add MDL File")) {}
                    if (ImGui::MenuItem("Create MDL Graph")) {}

                    ImGui::EndMenu();
                }
            }

            /**
            * @brief Begin render Physics.
            */
            {
                SPICES_PROFILE_ZONEN("Render Physics");

                if (ImGui::BeginMenu(ICON_TEXT(ICON_MD_WATERFALL_CHART, Physics)))
                {
                    if (ImGui::MenuItem("Physics Scene")) {}
                    if (ImGui::MenuItem("Ground Plane")) {}
                    if (ImGui::MenuItem("Physical Material")) {}
                    if (ImGui::MenuItem("Collision Group")) {}
                    if (ImGui::BeginMenu("Joint"))
                    {
                        if (ImGui::MenuItem("D6 Joint")) {}
                        if (ImGui::MenuItem("Fixed Joint")) {}
                        if (ImGui::MenuItem("Revolute Joint")) {}
                        if (ImGui::MenuItem("Prismatic Joint")) {}
                        if (ImGui::MenuItem("Spherical Joint")) {}
                        if (ImGui::MenuItem("Distance Joint")) {}
                        if (ImGui::MenuItem("Gear Joint")) {}
                        if (ImGui::MenuItem("Rack and Pinion Joint")) {}

                        ImGui::EndMenu();
                    }
                    if (ImGui::MenuItem("Particle System")) {}
                    if (ImGui::MenuItem("Vehicle")) {}
                    if (ImGui::MenuItem("Tire Friction Table")) {}

                    ImGui::EndMenu();
                }
            }

            /**
            * @brief Begin render Flow.
            */
            {
                SPICES_PROFILE_ZONEN("Render Flow");

                if (ImGui::BeginMenu(ICON_TEXT(ICON_MD_FLOOD, Flow)))
                {
                    if (ImGui::MenuItem("Sphere Emitter")) {}
                    if (ImGui::MenuItem("Box Emitter")) {}
                    if (ImGui::MenuItem("Point Emitter")) {}
                    if (ImGui::MenuItem("Mesh Emitter")) {}
                    if (ImGui::MenuItem("Texture Emitter")) {}
                    if (ImGui::MenuItem("NanoVDB Emitter")) {}
                    ImGui::Separator();
                    if (ImGui::MenuItem("Simulate Properties")) {}
                    if (ImGui::MenuItem("Offscreen Properties")) {}
                    if (ImGui::MenuItem("Render Properties")) {}
                    if (ImGui::MenuItem("Isosurface Properties")) {}

                    ImGui::EndMenu();
                }
            }

            

            /**
            * @brief Begin render Flow.
            */
            {
                SPICES_PROFILE_ZONEN("Render Particles");

                if (ImGui::BeginMenu(ICON_TEXT(ICON_MD_FORMAT_SHAPES, Particles)))
                {
                    if (ImGui::MenuItem("with Geometry Replicator")) {}
                    if (ImGui::MenuItem("with Point Instancer")) {}
                    if (ImGui::MenuItem("with Particle Visualizer")) {}
                    if (ImGui::MenuItem("Point Cloud Flow Emitter")) {}
                    if (ImGui::MenuItem("Material")) {}
                    if (ImGui::MenuItem("Sprite")) {}
                    if (ImGui::MenuItem("Mesh")) {}
                    if (ImGui::MenuItem("PointInstancer")) {}
                    if (ImGui::MenuItem("Prototypes")) {}
                    if (ImGui::MenuItem("Slate")) {}
                    if (ImGui::MenuItem("FlowEmitterPoint")) {}

                    ImGui::EndMenu();
                }
            }

            /**
            * @brief Begin render Sequence.
            */
            if (ImGui::MenuItem(ICON_TEXT(ICON_MD_VIDEO_CAMERA_FRONT, Sequence))) {}

            /**
            * @brief Begin render Navigation.
            */
            {
                SPICES_PROFILE_ZONEN("Render Navigation");

                if (ImGui::BeginMenu(ICON_TEXT(ICON_MD_NAVIGATION, Navigation)))
                {
                    if (ImGui::MenuItem("NavMesh Volume")) {}

                    ImGui::EndMenu();
                }
            }

            /**
            * @brief Begin render Visual Scripting.
            */
            {
                SPICES_PROFILE_ZONEN("Render Visual Scripting");

                if (ImGui::BeginMenu(ICON_TEXT(ICON_MD_JAVASCRIPT, Visual Scripting)))
                {
                    if (ImGui::MenuItem("Action Graph")) {}
                    if (ImGui::MenuItem("Push Graph")) {}
                    if (ImGui::MenuItem("Lazy Graph")) {}

                    ImGui::EndMenu();
                }
            }

            /**
            * @brief Begin render BasisCurves.
            */
            {
                SPICES_PROFILE_ZONEN("Render BasisCurves");

                if (ImGui::BeginMenu(ICON_TEXT(ICON_MD_POLYLINE, BasisCurves)))
                {
                    if (ImGui::MenuItem("Create Curve Deformer")) {}
                    if (ImGui::BeginMenu("Graph"))
                    {
                        if (ImGui::MenuItem("Curve from Xformables")) {}
                        if (ImGui::MenuItem("Extract Curve Data")) {}
                        if (ImGui::MenuItem("Tag Curves with Ramp")) {}
                        if (ImGui::MenuItem("Resample Curves")) {}

                        ImGui::EndMenu();
                    }
                    if (ImGui::MenuItem("New Curve")) {}

                    ImGui::EndMenu();
                }
            }

            /**
            * @brief Begin render Animation.
            */
            {
                SPICES_PROFILE_ZONEN("Render Animation");

                if (ImGui::BeginMenu(ICON_TEXT(ICON_MD_ANIMATION, Animation)))
                {
                    if (ImGui::MenuItem("Animation Graph")) {}

                    ImGui::EndMenu();
                }
            }

            /**
            * @brief Begin render UI.
            */
            {
                SPICES_PROFILE_ZONEN("Render UI");

                if (ImGui::BeginMenu(ICON_TEXT(ICON_MD_SQUARE, UI)))
                {
                    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.5f, 0.5f, 0.5f, 1.0f));
                    ImGui::SeparatorText("Containers");
                    ImGui::PopStyleColor();
                    if (ImGui::MenuItem(ICON_TEXT(ICON_EMPTY, Frame))) {}
                    if (ImGui::MenuItem(ICON_TEXT(ICON_EMPTY, ScrollingFrame))) {}
                    if (ImGui::MenuItem(ICON_TEXT(ICON_EMPTY, CollapsableFrame))) {}
                    if (ImGui::MenuItem(ICON_TEXT(ICON_EMPTY, HStack))) {}
                    if (ImGui::MenuItem(ICON_TEXT(ICON_EMPTY, VStack))) {}
                    if (ImGui::MenuItem(ICON_TEXT(ICON_EMPTY, ZStack))) {}
                    if (ImGui::MenuItem(ICON_TEXT(ICON_EMPTY, Placer))) {}
                    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.5f, 0.5f, 0.5f, 1.0f));
                    ImGui::SeparatorText("Widgets");
                    ImGui::PopStyleColor();

                    if (ImGui::MenuItem(ICON_TEXT(ICON_EMPTY, Button))) {}
                    if (ImGui::MenuItem(ICON_TEXT(ICON_EMPTY, Image))) {}
                    if (ImGui::MenuItem(ICON_TEXT(ICON_EMPTY, Rectangle))) {}
                    if (ImGui::MenuItem(ICON_TEXT(ICON_EMPTY, Spacer))) {}
                    if (ImGui::MenuItem(ICON_TEXT(ICON_EMPTY, Label))) {}
                    if (ImGui::MenuItem(ICON_TEXT(ICON_EMPTY, Line))) {}
                    if (ImGui::MenuItem(ICON_TEXT(ICON_EMPTY, Circle))) {}
                    if (ImGui::MenuItem(ICON_TEXT(ICON_EMPTY, Triangle))) {}
                    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.5f, 0.5f, 0.5f, 1.0f));
                    ImGui::SeparatorText("Style");
                    ImGui::PopStyleColor();

                    if (ImGui::MenuItem(ICON_TEXT(ICON_EMPTY, StyleContainer))) {}
                    if (ImGui::MenuItem(ICON_TEXT(ICON_EMPTY, Style))) {}
                    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.5f, 0.5f, 0.5f, 1.0f));
                    ImGui::SeparatorText("Window");
                    ImGui::PopStyleColor();

                    if (ImGui::MenuItem("Viewport Frame")) {}
                    if (ImGui::MenuItem("Window Frame")) {}

                    ImGui::EndMenu();
                }
            }

            /**
            * @brief Begin render AOV.
            */
            if (ImGui::MenuItem(ICON_TEXT(ICON_MD_INSERT_PHOTO, AOV))) {}


            /**
            * @brief End render Create.
            */
            ImGui::EndMenu();
        }
    }
}