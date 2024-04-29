#include "Pchheader.h"
#include "ImguiCreateEntity.h"
#include "Systems/SlateSystem.h"

namespace Spiecs {

    ImguiCreateEntity::ImguiCreateEntity(const std::string& panelName)
        : ImguiSlate(panelName)
    {
    }

    void ImguiCreateEntity::OnRender()
    {
        if (ImGui::BeginMenu("Create"))
        {
            // Mesh
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

            // Shape
            if (ImGui::BeginMenu("Mesh"))
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

            // Light
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

            // Audio
            if (ImGui::BeginMenu("Audio"))
            {
                if (ImGui::MenuItem("Spatial Sound")) {};
                if (ImGui::MenuItem("Non-Spatial Sound")) {};
                if (ImGui::MenuItem("Listener")) {};

                ImGui::EndMenu();
            }

            // Camera
            if (ImGui::MenuItem("Camera")) {};

            // Scope
            if (ImGui::MenuItem("Scope")) {};

            // Xform
            if (ImGui::MenuItem("Xform")) {};


            ImGui::EndMenu();
        }
    }
}