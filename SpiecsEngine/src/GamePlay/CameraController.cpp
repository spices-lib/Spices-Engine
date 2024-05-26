/**
* @file CameraController.cpp.
* @brief The CameraController Class Implementation.
* @author The Cherno & Spiecs.
*/

#include "Pchheader.h"
#include "CameraController.h"
#include "World/Entity.h"
#include "Render/FrameInfo.h"
#include "Core/Input/Input.h"
#include "Core/Input/MouseButtonCodes.h"
#include "Systems/SlateSystem.h"
#include "Core/Input/KeyCodes.h"

namespace Spiecs {

	void CameraController::OnConstruction()
	{
		Entity entity( m_Owner, FrameInfo::Get().m_World.get());
		m_CameraTranComp = &entity.GetComponent<TransformComponent>();
		m_Camera = entity.GetComponent<CameraComponent>().GetCamera();
	}

	void CameraController::OnTick(TimeStep& ts)
	{
		/**
		* @breif Only update view while viewport is hovered.
		*/
		if (!SlateSystem::GetRegister()->GetViewPort()->IsHovered()) return;

		if (Input::IsKeyPressed(Key::LeftAlt))
		{
			const glm::vec2& mouse{ Input::GetMouseX(), Input::GetMouseY() };
			glm::vec2 delta = (mouse - m_InitialMousePosition) * 0.003f;
			m_InitialMousePosition = mouse;
			
			if      (Input::IsMouseButtonPressed(Mouse::ButtonMiddle)) MousePan(delta);
			else if (Input::IsMouseButtonPressed(Mouse::ButtonLeft))   MouseRotate(delta);
			else if (Input::IsMouseButtonPressed(Mouse::ButtonRight))  MouseZoom(delta.y);

			UpdateView();
		}
	}

	void CameraController::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);

		dispatcher.Dispatch<KeyPressedEvent>(BIND_EVENT_FN(CameraController::OnKeyPressed));
		dispatcher.Dispatch<MouseScrolledEvent>(BIND_EVENT_FN(CameraController::OnMouseScroll));
		dispatcher.Dispatch<WindowResizeOverEvent>(BIND_EVENT_FN(CameraController::OnWindowResizeOver));
		//dispatcher.Dispatch<SlateResizeEvent>(BIND_EVENT_FN(CameraController::OnSlateResized));
	}

	bool CameraController::OnKeyPressed(KeyPressedEvent& e)
	{
		return false;
	}

	bool CameraController::OnMouseScroll(MouseScrolledEvent& e)
	{
		float delta = e.GetYOffest() * 0.1f;
		MouseZoom(delta);
		UpdateView();
		return false;
	}

	bool CameraController::OnWindowResizeOver(WindowResizeOverEvent& e)
	{
		float ratio = e.GetWidth() / float(e.GetHeight());
		std::any_cast<std::shared_ptr<Camera>>(m_Camera)->SetPerspective(ratio);

		return false;
	}
	
	bool CameraController::OnSlateResized(SlateResizeEvent& e)
	{
		float ratio = e.GetWidth() / float(e.GetHeight());
		std::any_cast<std::shared_ptr<Camera>>(m_Camera)->SetPerspective(ratio);
		return false;
	}

	void CameraController::MousePan(const glm::vec2& delta)
	{
		auto [xSpeed, ySpeed] = PanSpeed();
		m_FocalPoint += -GetRightDirection() * delta.x * xSpeed * m_Distance;
		m_FocalPoint += -GetUpDirection() * delta.y * ySpeed * m_Distance;
	}

	void CameraController::MouseRotate(const glm::vec2& delta)
	{
		glm::vec3 rot = std::any_cast<TransformComponent*>(m_CameraTranComp)->GetRotation();
		float yawSign = GetUpDirection().y < 0 ? -1.0f : 1.0f;

		rot.y -= yawSign * delta.x * RotationSpeed();
		rot.x += delta.y * RotationSpeed();

		std::any_cast<TransformComponent*>(m_CameraTranComp)->SetRotation(rot);
	}

	void CameraController::MouseZoom(float delta)
	{
		m_Distance += delta * ZoomSpeed();
		if (m_Distance < 1.0f)
		{
			m_FocalPoint += GetForwardDirection();
			m_Distance = 1.0f;
		}
	}

	std::pair<float, float> CameraController::PanSpeed() const
	{
		float x = std::min(m_ViewportWidth / 1000.0f, 2.4f); // max = 2.4f
		float xFactor = 0.0366f * (x * x) - 0.1778f * x + 0.3021f;

		float y = std::min(m_ViewportHeight / 1000.0f, 2.4f); // max = 2.4f
		float yFactor = 0.0366f * (y * y) - 0.1778f * y + 0.3021f;

		return { xFactor, yFactor };
	}

	float CameraController::RotationSpeed() const
	{
		return 0.8f;
	}

	float CameraController::ZoomSpeed() const
	{
		float distance = m_Distance * 0.2f;
		distance = std::max(distance, 0.0f);
		float speed = distance * distance;
		speed = std::min(speed, 100.0f); // max speed = 100
		return speed;
	}

	void CameraController::UpdateView()
	{
		glm::vec3 pos = CalculatePosition();
		std::any_cast<TransformComponent*>(m_CameraTranComp)->SetPostion(pos);
		// m_Yaw = m_Pitch = 0.0f; // Lock the camera's rotation
	}

	glm::vec3 CameraController::CalculatePosition() const
	{
		return m_FocalPoint - GetForwardDirection() * m_Distance;
	}

	glm::vec3 CameraController::GetUpDirection() const
	{
		return glm::rotate(GetOrientation(), glm::vec3(0.0f, -1.0f, 0.0f));
	}

	glm::vec3 CameraController::GetRightDirection() const
	{
		return glm::rotate(GetOrientation(), glm::vec3(1.0f, 0.0f, 0.0f));
	}

	glm::vec3 CameraController::GetForwardDirection() const
	{
		return glm::rotate(GetOrientation(), glm::vec3(0.0f, 0.0f, 1.0f));
	}

	glm::quat CameraController::GetOrientation() const
	{
		const glm::vec3& rot = std::any_cast<TransformComponent*>(m_CameraTranComp)->GetRotation();
		return glm::quat(glm::vec3(rot.x, rot.y, rot.z));
	}
}