/**
* @file CameraController.h.
* @brief The CameraController Class Definitions.
* @author The Cherno & Spices.
*/

#pragma once
#include "Core/Core.h"
#include "Camera.h"
#include "World/Components/NativeScriptComponent.h"
#include "World/Components/TransformComponent.h"

#include <any>

namespace Spices {

	/**
	* @brief CameraController Class.
	* This class inherit from NativeScriptComponent.
	* It receives Mouse Event and Key Event to control the Camera movement which defined by it's owner.
	*/
	class CameraController : public NativeScriptComponent
	{
	public:

		/**
		* @brief Constructor Function.
		*/
		CameraController() = default;

		/**
		* @brief Destructor Function.
		*/
		virtual ~CameraController() override = default;

		/**
		* @brief The interface inherited from Component, which is used for serialize this class.
		* @todo Implemented it.
		*/
		virtual void OnSerialize() override {}

		/**
		* @brief The interface inherited from Component, which is used for deserialize this class.
		* @todo Implemented it.
		*/
		virtual void OnDeSerialize() override {}

		/**
		* @brief The interface inherited from NativeScriptComponent, which is called on this attached to an entity.
		* We get the camera we want control here.
		* We get the camera transform component here.
		*/
		virtual void OnConstruction() override;

		/**
		* @brief The interface inherited from NativeScriptComponent, which is called every engine loop frame.
		* We implemented mainly camera control here, the reason is query input has better performance than event.
		*/
		virtual void OnTick(TimeStep& ts) override;

		/**
		* @brief The interface inherited from NativeScriptComponent, which is called on the global event function pointer called.
		*/
		virtual void OnEvent(Event& e) override;

		/**
		* @brief Get Camera Up(y) Direction.
		* @return The Camera Up(y) Direction.
		*/ 
		glm::vec3 GetUpDirection() const;

		/**
		* @brief Get Camera Right(x) Direction.
		* @return The Camera Right(x) Direction.
		*/
		glm::vec3 GetRightDirection() const;

		/**
		* @brief Get Camera Forward(z) Direction.
		* @return The Camera Forward(z) Direction.
		*/
		glm::vec3 GetForwardDirection() const;

		/**
		* @brief Get Camera quaternion rotation.
		* @return The Camera quaternion rotation.
		*/
		glm::quat GetOrientation() const;

	private:

		/**
		* @brief Event OnKeyPressed.
		* We do nothing here.
		* @param[in] e Event Wrapper.
		* @return true if we need block the event.
		* @todo Implemented it.
		*/
		bool OnKeyPressed(KeyPressedEvent& e);

		/**
		* @brief Event OnMouseScroll.
		* Scale the camera.
		* @param[in] e Event Wrapper.
		* @return true if we need block the event
		*/
		bool OnMouseScroll(MouseScrolledEvent& e);

		/**
		* @brief Event OnWindowResized.
		* Reset Camera aspect ratio.
		* @param[in] e Event Wrapper.
		* @return true if we need block the event
		*/
		bool OnSlateResized(SlateResizeEvent& e);

		/**
		* @brief Calculate Camera Drag.
		* @param[in] delta Mouse position delta on viewport during frames.
		*/
		void MousePan(const glm::vec2& delta);

		/**
		* @brief Calculate Camera Rotate.
		* @param[in] delta Mouse position delta on viewport during frames.
		*/
		void MouseRotate(const glm::vec2& delta) const;

		/**
		* @brief Calculate Camera Zoom.
		* @param[in] delta Mouse position delta y on viewport during frames.
		*/
		void MouseZoom(const float& delta);

		/**
		* @brief Calculate Camera Drag speed.
		* @return Returns the camera drag speed in x direction and y direction.
		*/
		std::pair<float, float> PanSpeed() const;

		/**
		* @brief Calculate Camera Rotate speed.
		* @return Returns the speed camera rotate.
		*/
		float RotationSpeed() const;

		/**
		* @brief Calculate Camera Zoom speed.
		* @return Returns the speed camera zoom.
		*/
		float ZoomSpeed() const;

		/**
		* @brief Setting camera transform component position.
		* We already set rotation in MouseRotate().
		*/
		void UpdateView() const;

		/**
		* @brief Calculate camera transform component position using the class member parameters.
		* @return Returns camera transform component position.
		*/
		glm::vec3 CalculatePosition() const;

	private:

		/**
		* @brief The camera smart pointer get from owner's camera component.
		*/
		std::shared_ptr<Camera> m_Camera;

		/**
		* @brief The camera transform component pointer get from owner's transform component.
		*/
		TransformComponent* m_CameraTranComp = nullptr;

		/**
		* @brief The Viewport size.
		* @todo Make it param.
		*/
		uint32_t m_ViewportWidth = 1920, m_ViewportHeight = 1080;

		/**
		* @brief Zoom Level, for orthographic type camera.
		*/
		float m_ZoomLevel = 1.0f;

		/**
		* @brief The camera spring arm.
		* Init with 10.
		*/
		float m_Distance = 10.0f;

		/**
		* @brief The mouse position.
		* Init with 0.
		*/
		glm::vec2 m_InitialMousePosition = { 0.0f, 0.0f };

		/**
		* @brief The focus point.
		* Init with 0.
		*/
		glm::vec3 m_FocalPoint = { 0.0f, 0.0f, 0.0f };
	};
}