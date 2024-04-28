/**
* @file SlateRegister.h.
* @brief The SlateRegister Class Implementation.
* @author Spiecs.
*/

#pragma once
#include "Core/Core.h"

// Slate Specific Class Header.
#include "Slate/Imgui/ImguiConsole.h"
#include "Slate/Imgui/ImguiContentBrowser.h"
#include "Slate/Imgui/ImguiProperty.h"
#include "Slate/Imgui/ImguiStage.h"
#include "Slate/Imgui/ImguiViewport.h"
#include "Slate/Imgui/ImguiMainMenu.h"

// STL Header
#include <memory>
#include <vector>
#include <any>

namespace Spiecs {

	/**
	* @brief This Class is the manager of all slate, it keeps all slate handle.
	*/
	class SlateRegister
	{
	public:

		/**
		* @brief Constructor Function.
		*/
		SlateRegister() {};

		/**
		* @brief Destructor Function.
		*/
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
		*  @brief This function is called on SlateRenderer::Render.
		*/
		void OnRender();

		/**
		* @brief This function will be called on global Event function pointer is called.
		*/
		void OnEvent(Event& event);

	private:
		
		/**
		* @brief The container of all slate handle.
		*/
		std::vector<std::unique_ptr<ImguiSlate>> m_Slates;
	};

	template<typename T, typename ...Args>
	inline void SlateRegister::Register(Args && ...args)
	{
		m_Slates.push_back(std::make_unique<T>(std::forward<Args>(args)...));
	}
}