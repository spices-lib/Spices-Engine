#pragma once
#include "Core/Core.h"

namespace Spiecs {

	class System
	{
	public:
		System(const std::string& systemName) : m_SystemName(systemName) {};
		virtual ~System() {};

		System(const System&) = delete;
		System& operator=(const System&) = delete;

		virtual void OnSystemInitialize() {};
		virtual void OnSystemShutDown() {};
		virtual void OnSystemUpdate() {};

	protected:
		std::string m_SystemName;
	};

	class SystemManager
	{
	public:
		SystemManager();
		virtual ~SystemManager() {};

		SystemManager(const SystemManager&) = delete;
		SystemManager& operator=(const SystemManager&) = delete;

		static SystemManager& Get();
		static void Run();

		template<typename T, typename ... Args>
		SystemManager& PushSystem(const std::string& systemName, Args&& ... args)
		{
			// push system to map
			if (m_Identities.find(systemName) != m_Identities.end())
			{
				std::cout << "ERROR: " << systemName << " has been pushed" << std::endl;
				__debugbreak();
			}

			m_Identities[systemName] = std::unique_ptr<System>(new T(systemName, std::forward<Args>(args)...));

			// system init
			m_Identities[systemName]->OnSystemInitialize();
			std::cout << "INFO: " << systemName << " pushed" << std::endl;

			return *m_SystemManager;
		}

		SystemManager& PopSystem(const std::string& systemName)
		{
			// pop system from map
			if (m_Identities.find(systemName) == m_Identities.end())
			{
				std::cout << "ERROR: " << systemName << " has been poped" << std::endl;
				__debugbreak();
			}

			// system shutdown
			m_Identities[systemName]->OnSystemShutDown();
			std::cout << "INFO: " << systemName << " poped" << std::endl;

			m_Identities.erase(systemName);

			return *m_SystemManager;
		}

	private:
		static std::unique_ptr<SystemManager> m_SystemManager;
		static std::unordered_map<std::string, std::unique_ptr<System>> m_Identities;
	};
}