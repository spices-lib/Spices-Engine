#pragma once
#include "Core/Core.h"
#include "Camera.h"
#include "World/Components/NativeScriptComponent.h"
#include "World/Components/TransformComponent.h"

#include <any>

namespace Spiecs {

	class CameraController : public NativeScriptComponent
	{
	public:
		CameraController() {};
		virtual ~CameraController() {};

		virtual void OnSerialize() override {};
		virtual void OnDeSerialize() override {};

		virtual void OnConstruction() override;
		virtual void OnTick(TimeStep& ts) override;
		virtual void OnEvent(Event& e) override;

		glm::vec3 GetUpDirection() const;
		glm::vec3 GetRightDirection() const;
		glm::vec3 GetForwardDirection() const;
		glm::quat GetOrientation() const;

	private:
		bool OnKeyPressed(KeyPressedEvent& e);
		bool OnMouseScroll(MouseScrolledEvent& e);
		bool OnWindowResized(WindowResizeEvent& e);

		void MousePan(const glm::vec2& delta);
		void MouseRotate(const glm::vec2& delta);
		void MouseZoom(float delta);

		std::pair<float, float> PanSpeed() const;
		float RotationSpeed() const;
		float ZoomSpeed() const;

		void UpdateView();
		glm::vec3 CalculatePosition() const;

	private:
		std::any m_Camera;
		std::any m_CameraTranComp;

		// TODO: param
		float m_ViewportWidth = 1920, m_ViewportHeight = 1080;
		float m_Distance = 10.0f;
		glm::vec2 m_InitialMousePosition = { 0.0f, 0.0f };
		glm::vec3 m_FocalPoint = { 0.0f, 0.0f, 0.0f };
	};
}