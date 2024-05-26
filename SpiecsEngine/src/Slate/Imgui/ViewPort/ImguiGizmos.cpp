#include "Pchheader.h"
#include "ImguiGizmos.h"
#include "Core/Math/Math.h"
#include "Core/Input/Input.h"
#include "Core/Input/KeyCodes.h"
#include "World/World/World.h"
#include "World/Entity.h"
#include "Systems/SlateSystem.h"

#include <glm/gtc/type_ptr.hpp>

namespace Spiecs {

    void ImguiGizmos::OnRender()
    {
        if (m_FrameInfo.m_PickEntityID.size() > 0)
        {
            ImGuizmo::SetOrthographic(false);
            ImGuizmo::SetDrawlist();

            auto& size = SlateSystem::GetRegister()->GetViewPort()->GetPanelSize();

            float windowWidth = (float)ImGui::GetWindowWidth();
            float windowHeight = (float)ImGui::GetWindowHeight();
            ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, size.x, size.y);

            //// Camera
            glm::mat4 viewMat = glm::mat4(1.0f);
            glm::mat4 projectionMat = glm::mat4(1.0f);

            auto& view = m_FrameInfo.m_World->GetRegistry().view<CameraComponent>();
            for (auto& e : view)
            {
                auto& [tComp, transComp] = m_FrameInfo.m_World->GetRegistry().get<CameraComponent, TransformComponent>(e);

                if (tComp.IsActived())
                {
                    /**
                    * @brief Viewmaterix is the inverse of camera's modelmatrix.
                    */
                    viewMat = glm::inverse(transComp.GetModelMatrix());
                    projectionMat = tComp.GetCamera()->GetPMatrix();

                    break;
                }
            }

            // Entity transform
            Entity entity((entt::entity)m_FrameInfo.m_PickEntityID.endk(), m_FrameInfo.m_World.get());
            auto& tc = entity.GetComponent<TransformComponent>();
            glm::mat4 model = tc.GetModelMatrix();

            // Snapping
            bool snap = Input::IsKeyPressed(Key::LeftControl);
            float snapValue = 0.5f; // Snap to 0.5m for translation/scale
            // Snap to 45 degrees for rotation
            if (m_GizmoType == ImGuizmo::OPERATION::ROTATE)
            {
                snapValue = 45.0F;
            }

            float snapValues[3] = { snapValue, snapValue ,snapValue };

            ImGuizmo::DrawGrid(glm::value_ptr(viewMat), glm::value_ptr(projectionMat), glm::value_ptr(glm::mat4(1.0f)), 100.f);
            ImGuizmo::Manipulate(glm::value_ptr(viewMat), glm::value_ptr(projectionMat),
                (ImGuizmo::OPERATION)m_GizmoType, ImGuizmo::LOCAL, glm::value_ptr(model),
                nullptr, snap ? snapValues : nullptr);

            if (ImGuizmo::IsUsing())
            {
                glm::vec3 translation, rotation, scale;
                DecomposeTransform(model, translation, rotation, scale);

                glm::vec3 deltaRotation = rotation - tc.GetRotation();
                tc.SetPostion(translation);
                tc.SetRotation(tc.GetRotation() + deltaRotation);
                tc.SetScale(scale);
            }


            /*ImGuiIO& io = ImGui::GetIO();
            float viewManipulateRight = io.DisplaySize.x;
            float viewManipulateTop = 0;

            ImGuizmo::SetDrawlist();
            float windowWidth = (float)ImGui::GetWindowWidth();
            float windowHeight = (float)ImGui::GetWindowHeight();
            ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, windowWidth, windowHeight);
            viewManipulateRight = ImGui::GetWindowPos().x + windowWidth;
            viewManipulateTop = ImGui::GetWindowPos().y;*/

            //ImGuizmo::DrawGrid(cameraView, cameraProjection, identityMatrix, 100.f);
            //ImGuizmo::DrawCubes(cameraView, cameraProjection, &objectMatrix[0][0], gizmoCount);
            //ImGuizmo::Manipulate(cameraView, cameraProjection, mCurrentGizmoOperation, mCurrentGizmoMode, matrix, NULL, useSnap ? &snap[0] : NULL, boundSizing ? bounds : NULL, boundSizingSnap ? boundsSnap : NULL);

            //ImGuizmo::ViewManipulate(cameraView, camDistance, ImVec2(viewManipulateRight - 128, viewManipulateTop), ImVec2(128, 128), 0x10101010);

        }
    }
}