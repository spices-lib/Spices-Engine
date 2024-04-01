#pragma once
#include "Core/Core.h"
#include "Component.h"
#include "World/Camera.h"

namespace Spiecs {

	class CameraComponent : public Component
	{
	public:
		CameraComponent() {};
		virtual ~CameraComponent() {};

		virtual void OnSerialize() override;
		virtual void OnDeSerialize() override;
		virtual void OnComponentAdded() override;

		void SetCamera(std::shared_ptr<Camera> camera) { m_Camera = camera; };
		std::shared_ptr<Camera> GetCamera() { return m_Camera; };

	private:
		std::shared_ptr<Camera> m_Camera;
	};
}