/**
* @file ImguiMainMenu.h
* @brief The ImguiMainMenu Class Definitions.
* @author Spiecs.
*/

#pragma once
#include "Core/Core.h"
#include "../ImguiUtils.h"

namespace Spiecs {

	/**
	* @brief Forward Declear.
	*/
	class ImguiWindow;

	/**
	* @brief Forward Declear.
	*/
	class ImguiCreateEntity;

	/**
	* @brief Forward Declear.
	*/
	class ImguiHelp;

	/**
	* @brief The ImguiMainMenu Class.
	* This class defines how to render a main menu.
	*/
	class ImguiMainMenu : public ImguiSlate
	{
	public:

		/**
		* @brief Constructor Function.
		* @param[in] panelName The Slate's name to show.
		* @param[in] frameInfo The Frame Date that in use.
		*/
		ImguiMainMenu(
			const std::string& panelName , 
			FrameInfo&         frameInfo
		);

		/**
		* @brief Destructor Function.
		*/
		virtual ~ImguiMainMenu() {};

		/**
		* @brief This interface is called On SlateSystem Update.
		* @param[in] ts TimeStep.
		*/
		virtual void OnUpdate(TimeStep& ts) override {};

		/**
		* @brief This interface is called On SlateRenderer Render.
		*/
		virtual void OnRender() override;

		/**
		* @brief This interface is called On Global Event Function Pointer is called.
		*/
		virtual void OnEvent(Event& event) override {};

	private:

		/**
		* @breaf The shared pointer of ImguiCreateEntity.
		*/
		std::shared_ptr<ImguiCreateEntity> m_Create;

		/**
		* @breaf The shared pointer of ImguiWindow.
		*/
		std::shared_ptr<ImguiWindow> m_Window;

		/**
		* @breaf The shared pointer of ImguiHelp.
		*/
		std::shared_ptr<ImguiHelp> m_Help;
	};

}