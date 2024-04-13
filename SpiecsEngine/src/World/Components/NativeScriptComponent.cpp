#include "Pchheader.h"
#include "NativeScriptComponent.h"

namespace Spiecs {

	NativeScriptComponent::NativeScriptComponent(std::shared_ptr<NativeScriptComponent> realComp)
	{
		m_RealNScriptComp = realComp;
	}

	void NativeScriptComponent::OnComponentAdded(entt::entity entity)
	{
		Component::OnComponentAdded(entity);

		/**
		* @brief Call Construction in RealNScripComponent
		*/
		OnConstruction();
	}

	void NativeScriptComponent::OnConstruction()
	{
		m_RealNScriptComp->m_Owner = m_Owner;

		m_RealNScriptComp->OnConstruction();
	}

	void NativeScriptComponent::OnTick(TimeStep& ts)
	{
		m_RealNScriptComp->OnTick(ts);
	}

	void NativeScriptComponent::OnEvent(Event& e)
	{
		m_RealNScriptComp->OnEvent(e);
	}
}