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
        ZoneScoped;

        /**
        * @brief Set Gizmo Projection type.
        */
        ImGuizmo::SetOrthographic(false);

        /**
        * @brief Use Window Draw List.
        */
        ImGuizmo::SetDrawlist();

        ImGuizmo::SetRect(
            m_Owner->GetPanelPos().x  ,
            m_Owner->GetPanelPos().y  ,
            m_Owner->GetPanelSize().x ,
            m_Owner->GetPanelSize().y
        );

        // Camera' Matrix.
        Entity CameraEntity;
        glm::mat4 viewMat       = glm::mat4(1.0f);
        glm::mat4 projectionMat = glm::mat4(1.0f);

        auto& view = m_FrameInfo.m_World->GetRegistry().view<CameraComponent>();
        for (auto& e : view)
        {
            auto& [tComp, transComp] = m_FrameInfo.m_World->GetRegistry().get<CameraComponent, TransformComponent>(e);

            if (tComp.IsActived())
            {
                viewMat = glm::inverse(transComp.GetModelMatrix());
                projectionMat = tComp.GetCamera()->GetPMatrix();

                CameraEntity = Entity(e, m_FrameInfo.m_World.get());
                break;
            }
        }

        /**
        * @brief Draw Editor Grid.
        */
        ImGuizmo::DrawGrid(
            glm::value_ptr(viewMat)         , 
            glm::value_ptr(projectionMat)   , 
            glm::value_ptr(glm::mat4(1.0f)) , 
            100.f
        );

        ImGuizmo::ViewManipulate(
            glm::value_ptr(viewMat), 
            8.0f, 
            ImVec2(m_Owner->GetPanelPos().x, m_Owner->GetPanelPos().y + m_Owner->GetPanelSize().y - 96),
            ImVec2(96, 96),
            0x10101010
        );

        glm::vec3 translation, rotation, scale;
        DecomposeTransform(glm::inverse(viewMat), translation, rotation, scale);

        auto& tc = CameraEntity.GetComponent<TransformComponent>();
        tc.SetPostion(translation);
        tc.SetRotation(rotation);
        tc.SetScale(scale);

        /**
        * @brief Draw Editor Gizmo.
        */
        if (m_FrameInfo.m_PickEntityID.size() > 0 && bEnableGizmo)
        {
            /**
            * @brief Get Selected Entity.
            */
            Entity entity(
                static_cast<entt::entity>(*m_FrameInfo.m_PickEntityID.endk()), 
                m_FrameInfo.m_World.get()
            );

            /**
            * @brief Get Transform Component.
            */
            auto& tc = entity.GetComponent<TransformComponent>();

            /**
            * @brief Get Entity Model Matrix.
            */
            glm::mat4 model = tc.GetModelMatrix();

            /**
            * @brief Snapping.
            */ 
            bool snap = Input::IsKeyPressed(Key::LeftControl);
            float snapValue = 0.5f; // Snap to 0.5m for translation/scale
            // Snap to 45 degrees for rotation
            if (m_GizmoType == ImGuizmo::OPERATION::ROTATE)
            {
                snapValue = 45.0F;
            }

            float snapValues[3] = { snapValue, snapValue ,snapValue };

            /**
            * @brief Gozmo Manipulater.
            */
            ImGuizmo::Manipulate(
                glm::value_ptr(viewMat)               , 
                glm::value_ptr(projectionMat)         ,
                (ImGuizmo::OPERATION)m_GizmoType      , 
                ImGuizmo::LOCAL                       , 
                glm::value_ptr(model)                 ,
                nullptr, snap ? snapValues : nullptr
            );

            /**
            * @brief Add to Entity Transform.
            */
            if (ImGuizmo::IsUsing())
            {
                glm::vec3 translation, rotation, scale;
                DecomposeTransform(model, translation, rotation, scale);

                glm::vec3 deltaRotation = rotation - tc.GetRotation();
                tc.SetPostion(translation);
                tc.SetRotation(tc.GetRotation() + deltaRotation);
                tc.SetScale(scale);
            }
        }
    }

    void ImguiGizmos::OnEvent(Event& event)
    {
        ZoneScoped;

        EventDispatcher dispatcher(event);

        dispatcher.Dispatch<KeyPressedEvent>(BIND_EVENT_FN(ImguiGizmos::OnKeyPressed));
    }

    bool ImguiGizmos::OnKeyPressed(KeyPressedEvent& e)
    {
        ZoneScoped;

        switch (e.GetKeyCode())
        {
        case Key::W:
            bEnableGizmo = true;
            m_GizmoType = ImGuizmo::OPERATION::TRANSLATE;
            break;
        case Key::E:
            bEnableGizmo = true;
            m_GizmoType = ImGuizmo::OPERATION::ROTATE;
            break;
        case Key::R:
            bEnableGizmo = true;
            m_GizmoType = ImGuizmo::OPERATION::SCALE;
            break;
        default:
            bEnableGizmo = false;
            m_GizmoType = -1;
            break;
        }

        return false;
    }
}