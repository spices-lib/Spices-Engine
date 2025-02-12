/**
* @file NativeScriptSystem.h.
* @brief The NativeScriptSystem Class Definitions.
* @author Spices.
*/

#pragma once
#include "Core/Core.h"
#include "SystemManager.h"
#include "Scripts/NativeScript.h"
#include <memory>

namespace Spices {

	/**
	* @brief NativeScriptSystem Class.
	* This class defines the specific behaves of NativeScriptSystem.
	*/
	class NativeScriptSystem : public System
	{
	public:

		/**
		* @brief Constructor Function.
		* Init class variable.
		* Usually call it.
		* @param[in] systemName The System name.
		*/
		NativeScriptSystem(const std::string& systemName) : System(systemName) {}

		/**
		* @brief Destructor Function.
		*/
		virtual ~NativeScriptSystem() override = default;

		/**
		* @brief This interface defines the behave on specific system initialized.
		* Called when system Pushed to SystemManager.
		*/
		virtual void OnSystemInitialize() override;

		/**
		* @brief This interface defines the behave on specific system shutdown.
		* Called when system poped from SystemManager.
		*/
		virtual void OnSystemShutDown() override;

		/**
		* @brief This interface defines the behave on specific system updated every frame.
		* @param[in] ts TimeStep.
		*/
		virtual void OnSystemUpdate(TimeStep& ts) override;

		/**
		* @brief This interface defines the behave on global event function pointer is called.
		* @param[in] event Event.
		*/
		virtual void OnEvent(Event& event) override;

	private:

		/**
		* @brief NativeScript Register.
		*/
		std::unique_ptr<NativeScriptRegister> m_NativeScriptRegister;
	};
}