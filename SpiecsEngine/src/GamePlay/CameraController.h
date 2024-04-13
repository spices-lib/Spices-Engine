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

	private:
		bool OnKeyPressed(KeyPressedEvent& e);

	private:
		std::any m_Camera;
		std::any m_CameraTranComp;
	};
}