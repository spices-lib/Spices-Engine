/**
* @file SystemManager.cpp.
* @brief The System Class and SystemManager Class Implementation.
* @author Spices.
*/

#include "Pchheader.h"
#include "SystemManager.h"
#include "Core/Event/Event.h"

namespace Spices {

	/**
	* @brief Defines the static SystemManager variable.
	*/
	std::unique_ptr<SystemManager> SystemManager::m_SystemManager;

	/**
	* @brief Defines the static System Map variable.
	*/
	scl::linked_unordered_map<std::string, std::shared_ptr<System>> SystemManager::m_Identities;

	SystemManager::SystemManager()
	{
		//m_SystemManager = std::unique_ptr<SystemManager>(this);
		m_Identities.clear();

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
		SPICES_PROFILE_ZONE;

		m_Identities.for_each([&](auto& k, auto& v) {
			v->OnSystemUpdate(ts);
			return false;
		});
	}

	void SystemManager::OnEvent(Event& event)
	{
		m_Identities.for_each([&](auto& k, auto& v) {
			v->OnEvent(event);
			return false;
		});
	}
}