/**
* @file SlateSystem.h.
* @brief The SlateSystem Class Definitions.
* @author Spices.
*/

#pragma once
#include "Core/Core.h"
#include "SystemManager.h"
#include "Slate/SlateRegister.h"

#include <memory>

namespace Spices {

	/**
	* @brief SlateSystem Class.
	* This class defines the specific behaves of SlateSystem.
	*/
	class SlateSystem : public System
	{
	public:

		/**
		* @brief Constructor Function.
		* Init class variable.
		* Usually call it.
		* @param[in] systemName The System name.
		*/
		SlateSystem(const std::string& systemName)
			: System(systemName)
		{}

		/**
		* @brief Destructor Function.
		*/
		virtual ~SlateSystem() override = default;

		/**
		* @brief This interface defines the behaves on specific system initialized.
		* Called when system Pushed to SystemManager.
		*/
		virtual void OnSystemInitialize() override;

		/**
		* @brief This interface defines the behaves on specific system shutdown.
		* Called when system popped from SystemManager.
		*/
		virtual void OnSystemShutDown() override;

		/**
		* @brief This interface defines the behaves on specific system updated every frame.
		* @param[in] ts TimeStep.
		*/
		virtual void OnSystemUpdate(TimeStep& ts) override;

		/**
		* @brief This interface defines the behaves on global event function pointer is called.
		* @param[in] event Event.
		*/
		virtual void OnEvent(Event& event) override;

		/**
		* @brief Get slate register.
		* @return Returns the shared pointer of slate register.
		*/
		static std::shared_ptr<SlateRegister> GetRegister() { return m_SlateRegister; }

	private:

		/**
		* @brief The slate register.
		*/
		static std::shared_ptr<SlateRegister> m_SlateRegister;
	};
}