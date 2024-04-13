#pragma once
#include "Core/Core.h"
#include "Component.h"
#include "GamePlay/Camera.h"

namespace Spiecs {

	class CameraComponent : public Component
	{
	public:
		CameraComponent(bool isActived = false) : m_IsActived(isActived) {};
		virtual ~CameraComponent() {};

		virtual void OnSerialize() override;
		virtual void OnDeSerialize() override;

		void SetCamera(std::shared_ptr<Camera> camera) { m_Camera = camera; };
		std::shared_ptr<Camera> GetCamera() { return m_Camera; };
		bool IsActived() { return m_IsActived; };

	private:
		std::shared_ptr<Camera> m_Camera;
		bool m_IsActived = false;
	};
}