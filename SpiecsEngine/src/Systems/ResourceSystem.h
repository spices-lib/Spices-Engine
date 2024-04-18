/**
* @file ResourceSystem.h.
* @brief The ResourceSystem Class Definitions.
* @author Spiecs.
*/

#pragma once
#include "Core/Core.h"
#include "SystemManager.h"

namespace Spiecs {

	/**
	* @brief ResourceSystem Class.
	* Handles resource load/unload event.
	* @todo Resource garbage collection
	*/
	class ResourceSystem : public System
	{
	public:

		/**
		* @brief Constructor Function.
		* Init class variable.
		* Usually call it.
		* @param[in] systemName The System name.
		*/
		ResourceSystem(const std::string& systemName) : System(systemName) {};

		/**
		* @brief Destructor Function.
		*/
		virtual ~ResourceSystem() override {};

		/**
		* @brief This interface defines the behaver on specific system initialized.
		* Called when system Pushed to SystemManager.
		*/
		virtual void OnSystemInitialize() override;

		/**
		* @brief This interface defines the behaver on specific system shutdown.
		* Called when system poped from SystemManager.
		*/
		virtual void OnSystemShutDown() override;

		/**
		* @brief This interface defines the bahaver on specific system updated every frame.
		* @param[in] ts TimeStep.
		*/
		virtual void OnSystemUpdate(TimeStep& ts) override;

		/**
		* @brief This interface defines the bahaver on golbal event function pointer is called.
		* @param[in] event Event.
		*/
		virtual void OnEvent(Event& event) override;

	private:

	};
}