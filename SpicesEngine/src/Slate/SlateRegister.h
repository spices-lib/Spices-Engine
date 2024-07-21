/**
* @file SlateRegister.h.
* @brief The SlateRegister Class Implementation.
* @author Spices.
*/

#pragma once
#include "Core/Core.h"

// Slate Specific Class Header.
#include "Slate/Imgui/ImguiConsole.h"
#include "Slate/Imgui/ImguiContentBrowser.h"
#include "Slate/Imgui/ImguiProperty.h"
#include "Slate/Imgui/ImguiStage.h"
#include "Slate/Imgui/ViewPort/ImguiViewport.h"
#include "Slate/Imgui/MainMenu/ImguiMainMenu.h"
#include "Slate/Imgui/ViewPort/ImguiFloattingInfo.h"
#include "Slate/Imgui/ViewPort/ImguiGizmos.h"
#include "Slate/Imgui/ImguiOperation.h"
#include "Slate/Imgui/ImguiInfoBar.h"
#include "Slate/Imgui/ImguiMaterialPanel.h"

// STL Header
#include <memory>
#include <vector>
#include <any>
#include <sstream>

namespace Spices {

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
		* @tparam T Slate specific type.
		* @param[in] isPrimary Is this slate's render function is called by register or owner.
		* @param[in] panelName The slate name.
		* @return Returns the shared pointer of T.
		*/
		template<typename T, typename ... Args>
		std::shared_ptr<T> Register(bool isPrimary, const std::string& panelName, Args&& ... args);

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
		* @param[in] event The Specific Event Type.
		*/
		void OnEvent(Event& event);

		/**
		* @breif Get Viewport Slate.
		* @return Returns the shared pointer of ImguiViewport.
		*/
		std::shared_ptr<ImguiViewport> GetViewPort();

	private:
		
		/**
		* @brief The container of all slate handle.
		*/
		std::unordered_map<std::string, std::shared_ptr<ImguiSlate>> m_SlatesEventContainer;

		/**
		* @brief The container of all slate handle.
		*/
		std::unordered_map<std::string, std::shared_ptr<ImguiSlate>> m_SlatesRenderContainer;
	};

	template<typename T, typename ...Args>
	inline std::shared_ptr<T> SlateRegister::Register(bool isPrimary, const std::string& panelName, Args && ...args)
	{
		SPICES_PROFILE_ZONE;

		/**
		* @brief Not allow registy repeat.
		*/
		if (m_SlatesEventContainer.find(panelName) != m_SlatesEventContainer.end())
		{
			std::stringstream ss;
			ss << panelName << " Slate already registed, please check your code again.";

			SPICES_CORE_ERROR(ss.str());
		}

		/**
		* @breif Instance T and store it.
		*/
		std::shared_ptr<T> _T = std::make_shared<T>(panelName, FrameInfo::Get(), std::forward<Args>(args)...);
		m_SlatesEventContainer[panelName] = _T;

		if(isPrimary) m_SlatesRenderContainer[panelName] = _T;

		return _T;
	}
}