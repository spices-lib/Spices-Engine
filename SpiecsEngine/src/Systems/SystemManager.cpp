#include "Pchheader.h"
#include "SystemManager.h"
#include "Core/Event/Event.h"

namespace Spiecs {

	std::unique_ptr<SystemManager> SystemManager::m_SystemManager;
	std::unordered_map<std::string, std::unique_ptr<System>> SystemManager::m_Identities;

	SystemManager::SystemManager()
	{
		//m_SystemManager = std::unique_ptr<SystemManager>(this);
		m_Identities.empty();

		/**
		* @brief Set Golbal EventCallBack
		*/
		Event::SetEventCallbackFn(BIND_EVENT_FN(SystemManager::OnEvent));
	}

	SystemManager& SystemManager::Get()
	{
		return *m_SystemManager;
	}

	void SystemManager::Run(TimeStep& ts)
	{
		for (auto& pair : m_Identities)
		{
			pair.second->OnSystemUpdate(ts);
		}
	}

	void SystemManager::OnEvent(Event& event)
	{
		for (auto& pair : m_Identities)
		{
			pair.second->OnEvent(event);
		}
	}
}