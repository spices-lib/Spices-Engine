/**
* @file CameraComponent.h.
* @brief The CameraComponent Class Definitions.
* @author Spiecs.
*/

#pragma once
#include "Core/Core.h"
#include "Component.h"
#include "GamePlay/Camera.h"

namespace Spiecs {

	/**
	* @brief CameraComponent Class.
	* This class defines the specific behaves of CameraComponent.
	*/
	class CameraComponent : public Component
	{
	public:

		/**
		* @brief Constructor Function.
		* Init class variable.
		* Usually call it.
		* @param[in] isActive Whether this camera component's camera is a primary camera.
		*/
		CameraComponent(bool isActive = false);

		/**
		* @brief Destructor Function.
		*/
		virtual ~CameraComponent() override = default;

		/**
		* @brief This interface defines how to serialize.
		* @todo Finish it.
		*/
		virtual void OnSerialize() override;

		/**
		* @brief This interface defines how to deserialize.
		* @todo Finish it.
		*/
		virtual void OnDeSerialize() override;

		/**
		* @brief This interface defines how to draw this component to property panel.
		*/
		virtual void DrawThis() override;
		
		/**
		* @brief Set the camera this component handled.
		* @param[in] camera The camera shared pointer.
		*/
		void SetCamera(std::shared_ptr<Camera> camera) { m_Camera = camera; }

		/**
		* @brief Get the camera variable.
		* @return Returns the camera variable.
		*/
		std::shared_ptr<Camera> GetCamera() { return m_Camera; }

		/**
		* @brief Query whether this camera is primary.
		* @return Returns true if id primary.
		*/
		bool IsActive() const { return m_IsActive; }

	private:

		/**
		* @brief The camera this component handled.
		*/
		std::shared_ptr<Camera> m_Camera;

		/**
		* @brief True if the camera is primary.
		*/
		bool m_IsActive = false;
	};
}