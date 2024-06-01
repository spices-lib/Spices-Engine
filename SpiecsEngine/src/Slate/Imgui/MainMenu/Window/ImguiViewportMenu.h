/**
* @file ImguiViewportMenu.h
* @brief The ImguiViewportMenu Class Definitions.
* @author Spiecs.
*/

#pragma once
#include "Core/Core.h"
#include "Slate/Imgui/ImguiUtils.h"

#define MaxViewportNum 2

namespace Spiecs {

	/**
	* @brief The ImguiViewportMenu Class.
	* This class defines how to render a ImguiViewport menu.
	*/
	class ImguiViewportMenu : public ImguiSlate
	{
	public:

		/**
		* @brief Constructor Function.
		* @param[in] panelName The Slate's name to show.
		* @param[in] frameInfo The Frame Date that in use.
		*/
		ImguiViewportMenu(
			const std::string& panelName,
			FrameInfo& frameInfo
		);

		/**
		* @brief Destructor Function.
		*/
		virtual ~ImguiViewportMenu() {};

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
		std::array<bool, MaxViewportNum> m_ViewportVisible;
	};
}