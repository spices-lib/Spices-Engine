/**
* @file ImguiCreateEntity.cpp.
* @brief The ImguiCreateEntity Class Implementation.
* @author Spiecs.
*/

#include "Pchheader.h"
#include "ImguiCreateEntity.h"

#include "Systems/SlateSystem.h"

namespace Spiecs {

    ImguiCreateEntity::ImguiCreateEntity(
        const std::string& panelName , 
        FrameInfo&         frameInfo
    )
        : ImguiSlate(panelName, frameInfo)
    {}

    void ImguiCreateEntity::OnRender()
    {
        SPIECS_PROFILE_ZONE;

        /**
        * @brief Begin render Create.
        */
        if (ImGui::BeginMenu("Create"))
        {
            /**
            * @brief Begin render Mesh.
            */
            {
                SPIECS_PROFILE_ZONEN("Render Mesh");

                if (ImGui::BeginMenu("Mesh"))
                {
                    if (ImGui::MenuItem("Cone")) {};
                    if (ImGui::MenuItem("Cube")) {};
                    if (ImGui::MenuItem("Cylinder")) {};
                    if (ImGui::MenuItem("Disk")) {};
                    if (ImGui::MenuItem("Plane")) {};
                    if (ImGui::MenuItem("Sphere")) {};
                    if (ImGui::MenuItem("Torus")) {};
                    ImGui::Separator();
                    if (ImGui::MenuItem("Settings")) {};

                    ImGui::EndMenu();
                }
            }
            
            /**
            * @brief Begin render Shape.
            */
            {
                SPIECS_PROFILE_ZONEN("Render Shape");

                if (ImGui::BeginMenu("Shape"))
                {
                    if (ImGui::MenuItem("Capsule")) {};
                    if (ImGui::MenuItem("Cone")) {};
                    if (ImGui::MenuItem("Cube")) {};
                    if (ImGui::MenuItem("Cylinder")) {};
                    if (ImGui::MenuItem("Sphere")) {};
                    ImGui::Separator();
                    bool isHighQuality = true;
                    if (ImGui::Checkbox("High Quality", &isHighQuality)) {};

                    ImGui::EndMenu();
                }
            }

            /**
            * @brief Begin render Light.
            */
            {
                SPIECS_PROFILE_ZONEN("Render Light");

                if (ImGui::BeginMenu("Light"))
                {
                    if (ImGui::MenuItem("Cylinder Light")) {};
                    if (ImGui::MenuItem("Disk Light")) {};
                    if (ImGui::MenuItem("Distance Light")) {};
                    if (ImGui::MenuItem("Dome Light")) {};
                    if (ImGui::MenuItem("Rect Light")) {};
                    if (ImGui::MenuItem("Sphere Light")) {};

                    ImGui::EndMenu();
                }
            }

            /**
            * @brief Begin render Audio.
            */
            {
                SPIECS_PROFILE_ZONEN("Render Audio");

                if (ImGui::BeginMenu("Audio"))
                {
                    if (ImGui::MenuItem("Spatial Sound")) {};
                    if (ImGui::MenuItem("Non-Spatial Sound")) {};
                    if (ImGui::MenuItem("Listener")) {};

                    ImGui::EndMenu();
                }
            }

            /**
            * @brief Begin render Camera.
            */
            {
                SPIECS_PROFILE_ZONEN("Render Camera");

                if (ImGui::MenuItem("Camera")) {};
            }
            /**
            * @brief Begin render Scope.
            */
            {
                SPIECS_PROFILE_ZONEN("Render Scope");

                if (ImGui::MenuItem("Scope")) {};
            }
            /**
            * @brief Begin render Xform.
            */
            {
                SPIECS_PROFILE_ZONEN("Render Xform");

                if (ImGui::MenuItem("Xform")) {};
            }

            /**
            * @brief End render Create.
            */
            ImGui::EndMenu();
        }
    }
}