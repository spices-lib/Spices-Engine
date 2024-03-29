#include "pchheader.h"
#include "SystemManager.h"

namespace Spiecs {

	std::unique_ptr<SystemManager> SystemManager::m_SystemManager;
	std::unordered_map<std::string, System*> SystemManager::m_Identities;

	SystemManager::SystemManager()
	{
		m_SystemManager = std::unique_ptr<SystemManager>(this);
		m_Identities.empty();
	}

	SystemManager& SystemManager::Get()
	{
		return *m_SystemManager;
	}

	void SystemManager::Run()
	{
		m_Identities["RenderSystem"]->OnSystemUpdate();
	}
}