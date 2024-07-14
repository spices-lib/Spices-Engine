/**
* @file ImguiMainMenu.h
* @brief The ImguiMainMenu Class Definitions.
* @author Spices.
*/

#pragma once
#include "Core/Core.h"
#include "Slate/Imgui/ImguiUtils.h"

namespace Spices {

	/**
	* @brief Forward Declear.
	*/
	class ImguiVisualizer;

	/**
	* @brief Forward Declear.
	*/
	class ImguiUtilities;

	/**
	* @brief Forward Declear.
	*/
	class ImguiViewportMenu;

	/**
	* @brief The ImguiWindow Class.
	* This class defines how to render a window menu.
	*/
	class ImguiWindow : public ImguiSlate
	{
	public:

		/**
		* @brief Constructor Function.
		* @param[in] panelName The Slate's name to show.
		* @param[in] frameInfo The Frame Date that in use.
		*/
		ImguiWindow(
			const std::string& panelName ,
			FrameInfo&         frameInfo
		);

		/**
		* @brief Destructor Function.
		*/
		virtual ~ImguiWindow() {};

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
		* @brief The shared pointer of ImguiVisualizer.
		*/
		std::shared_ptr<ImguiVisualizer> m_Visualizer;

		/**
		* @brief The shared pointer of ImguiUtilities.
		*/
		std::shared_ptr<ImguiUtilities> m_Utilities;

		/**
		* @brief The shared pointer of ImguiUtilities.
		*/
		std::shared_ptr<ImguiViewportMenu> m_ViewportMenu;
	};
}