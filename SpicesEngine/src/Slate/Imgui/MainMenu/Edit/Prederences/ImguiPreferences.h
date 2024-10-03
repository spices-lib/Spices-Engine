/**
* @file ImguiPreferences.h
* @brief The ImguiPreferences Class Definitions.
* @author Spices.
*/

#pragma once
#include "Core/Core.h"
#include "Slate/Imgui/ImguiUtils.h"

namespace Spices {

	/**
	* @brief The ImguiPreferences Class.
	* This class defines how to render a edit menu.
	*/
	class ImguiPreferences : public ImguiSlate
	{
	public:

		/**
		* @brief Constructor Function.
		* @param[in] panelName The Slate's name to show.
		* @param[in] frameInfo The Frame Date that in use.
		*/
		ImguiPreferences(
			const std::string& panelName ,
			FrameInfo&         frameInfo
		);

		/**
		* @brief Destructor Function.
		*/
		virtual ~ImguiPreferences() = default;

		/**
		* @brief This interface is called On SlateSystem Update.
		* @param[in] ts TimeStep.
		*/
		virtual void OnUpdate(TimeStep& ts) override {}

		/**
		* @brief This interface is called On SlateRenderer Render.
		*/
		virtual void OnRender() override;

		/**
		* @brief This interface is called On Global Event Function Pointer is called.
		*/
		virtual void OnEvent(Event& event) override {}
	};

}