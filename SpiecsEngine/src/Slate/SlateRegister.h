#pragma once
#include "Core/Core.h"

#include "Slate/Imgui/ImguiConsole.h"
#include "Slate/Imgui/ImguiContentBrowser.h"
#include "Slate/Imgui/ImguiProperty.h"
#include "Slate/Imgui/ImguiStage.h"
#include "Slate/Imgui/ImguiViewport.h"
#include "Slate/Imgui/ImguiMainMenu.h"

#include <memory>
#include <vector>
#include <any>

namespace Spiecs {

	class SlateRegister
	{
	public:
		SlateRegister() {};
		virtual ~SlateRegister() {};

		/**
		* @brief Copy Constructor Function.
		* @note This Class not allowed copy behaver.
		*/
		SlateRegister(const SlateRegister&) = delete;

		/**
		* @brief Copy Assignment Operation.
		* @note This Class not allowed copy behaver.
		*/
		SlateRegister& operator=(const SlateRegister&) = delete;

		template<typename T, typename ... Args>
		void Register(Args&& ... args);

		void OnUpdate(TimeStep& ts);
		void OnRender();
		void OnEvent(Event& event);

	private:
		
		std::vector<std::unique_ptr<ImguiSlate>> m_Slates;
	};

	template<typename T, typename ...Args>
	inline void SlateRegister::Register(Args && ...args)
	{
		m_Slates.push_back(std::make_unique<T>(std::forward<Args>(args)...));
	}
}