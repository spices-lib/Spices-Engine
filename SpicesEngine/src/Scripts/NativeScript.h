#pragma once
#include "Core/Core.h"
#include "Core/Event/Event.h"

namespace Spices {

	class NativeScript
	{
	public:
		NativeScript() {};
		virtual ~NativeScript() {};

		/**
		* @brief This interface defines the behaver on specific component tick every frame.
		* @param[in] ts TimeStep.
		*/
		virtual void OnTick(TimeStep& ts) = 0;

		/**
		* @brief This interface defines the behaver on specific component event happened.
		* @param[in] e Event.
		*/
		virtual void OnEvent(Event& e) = 0;
	};

	class NativeScriptRegister
	{
	public:

		/**
		* @brief Constructor Function.
		*/
		NativeScriptRegister() {};

		/**
		* @brief Destructor Function.
		*/
		virtual ~NativeScriptRegister() {};

		/**
		* @brief Copy Constructor Function.
		* @note This Class not allowed copy behaver.
		*/
		NativeScriptRegister(const NativeScriptRegister&) = delete;

		/**
		* @brief Copy Assignment Operation.
		* @note This Class not allowed copy behaver.
		*/
		NativeScriptRegister& operator=(const NativeScriptRegister&) = delete;

		/**
		* @brief This function regist slate instance.
		* @param[in] T Slate specific type.
		*/
		template<typename T, typename ... Args>
		void Register(Args&& ... args);

		/**
		* @note This function is not in use now.
		* @param[in] ts TimeStep.
		*/
		void OnUpdate(TimeStep& ts);

		/**
		* @brief This function will be called on global Event function pointer is called.
		*/
		void OnEvent(Event& event);

	private:

		/**
		* @brief The container of all native script handle.
		*/
		std::vector<std::unique_ptr<NativeScript>> m_NativeScripts;
	};

	template<typename T, typename ...Args>
	inline void NativeScriptRegister::Register(Args && ...args)
	{
		m_NativeScripts.push_back(std::make_unique<T>(std::forward<Args>(args)...));
	}
}