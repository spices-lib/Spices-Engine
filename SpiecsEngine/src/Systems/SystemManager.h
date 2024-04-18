/**
* @file SystemManager.h.
* @brief The System Class and SystemManager Class Definitions.
* @author Spiecs.
*/

#pragma once
#include "Core/Core.h"
#include "Core/Library/StringLibrary.h"

namespace Spiecs {

	/**
	* @brief Forward Declear.
	*/
	class Event;

	/**
	* @brief System Class.
	* This class defines the basic behaver of System.
	* When we create an new System, we need inherit from this.
	*/
	class System
	{
	public:

		/**
		* @brief Constructor Function.
		* Init class variable.
		* Usually call it.
		* @param[in] systemName The system's name.
		*/
		System(const std::string& systemName) : m_SystemName(systemName) {};

		/**
		* @brief Destructor Function.
		*/
		virtual ~System() {};

		/**
		* @brief Copy Constructor Function.
		* @note This Class not allowed copy behaver.
		*/
		System(const System&) = delete;

		/**
		* @brief Copy Assignment Operation.
		* @note This Class not allowed copy behaver.
		*/
		System& operator=(const System&) = delete;

		/**
		* @brief This interface defines the behaver on specific system initialized.
		* Called when system Pushed to SystemManager.
		*/
		virtual void OnSystemInitialize() {};

		/**
		* @brief This interface defines the behaver on specific system shutdown.
		* Called when system poped from SystemManager.
		*/
		virtual void OnSystemShutDown() {};

		/**
		* @brief This interface defines the bahaver on specific system updated every frame.
		* @param[in] ts TimeStep.
		*/
		virtual void OnSystemUpdate(TimeStep& ts) {};

		/**
		* @brief This interface defines the bahaver on golbal event function pointer is called.
		* @param[in] event Event.
		*/
		virtual void OnEvent(Event& event) {};

	protected:

		/**
		* @brief Specific system name.
		*/
		std::string m_SystemName;
	};

	/**
	* @brief SystemManager Class.
	* This class defines the behaver of SystemManager.
	*/
	class SystemManager
	{
	public:

		/**
		* @brief Using Type instead std::reference_wrapper<const std::type_info>.
		*/
		using Type = std::reference_wrapper<const std::type_info>;
	public:

		/**
		* @brief Constructor Function.
		*/
		SystemManager();

		/**
		* @brief Destructor Function.
		*/
		virtual ~SystemManager() {};

		/**
		* @brief Copy Constructor Function.
		* @note This Class not allowed copy behaver.
		*/
		SystemManager(const SystemManager&) = delete;

		/**
		* @brief Copy Assignment Operation.
		* @note This Class not allowed copy behaver.
		*/
		SystemManager& operator=(const SystemManager&) = delete;

		/**
		* @brief Get Static SystemManager.
		* @return Returns the Static SystemManager.
		*/
		static SystemManager& Get();

		/**
		* @brief Update all system that pushed to this manager. 
		* @param[in] ts TimeStep.
		*/
		static void Run(TimeStep& ts);

		/**
		* @brief The root event function pointer.
		* @param[in] event Event.
		*/
		void OnEvent(Event& event);

		/**
		* @brief Push a system to this mamager.
		* @param[in] T Specific system Class.
		* @return Returns the SystemManager.
		*/
		template<typename T, typename ... Args>
		SystemManager& PushSystem(Args&& ... args)
		{
			Type type = typeid(T);
			const std::vector<std::string>& outSplit = StringLibrary::SplitString(type.get().name(), ':');
			std::string systemName = outSplit[outSplit.size() - 1];

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

		/**
		* @brief Push a system to this mamager.
		* @param[in] systemName Specific system name.
		* @return Returns the SystemManager.
		*/
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

		/**
		* @brief Static SystemManager variable.
		*/
		static std::unique_ptr<SystemManager> m_SystemManager;

		/**
		* @brief Static System Map.
		*/
		static std::unordered_map<std::string, std::unique_ptr<System>> m_Identities;
	};
}