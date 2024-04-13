#pragma once
#include "Core/core.h"
#include "Component.h"
#include "Core/Event/Event.h"
#include "Core/Event/KeyEvent.h"
#include "Core/Event/MouseEvent.h"
#include "Core/Event/WindowEvent.h"

namespace Spiecs {

	class NativeScriptComponent : public Component
	{
	public:
		NativeScriptComponent() {};

		/**
		* @breif realcomponent Constructor
		*/
		NativeScriptComponent(std::shared_ptr<NativeScriptComponent> realComp);
		virtual ~NativeScriptComponent() {};

		virtual void OnSerialize() override {};
		virtual void OnDeSerialize() override {};
		virtual void OnComponentAdded(entt::entity entity) override;

		virtual void OnConstruction();
		virtual void OnTick(TimeStep& ts);
		virtual void OnEvent(Event& e);

	private:
		/**
		* @brief realcomponent
		*/
		std::shared_ptr<NativeScriptComponent> m_RealNScriptComp;
	};
}