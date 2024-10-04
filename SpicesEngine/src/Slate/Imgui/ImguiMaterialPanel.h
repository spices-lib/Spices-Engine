/**
* @file ImguiMaterialPanel.h
* @brief The ImguiMaterialPanel Class Definitions.
* @author Spices.
*/

#pragma once
#include "Core/Core.h"
#include "ImguiUtils.h"

#include "World/Entity.h"

namespace Spices {

	/**
	* @brief The ImguiMaterialPanel Class.
	* This class defines how to render a Material Property.
	*/
	class ImguiMaterialPanel : public ImguiSlate
	{
	public:

		/**
		* @brief Constructor Function.
		* @param[in] panelName The Slate's name to show.
		* @param[in] frameInfo The Frame Date that in use.
		*/
		ImguiMaterialPanel(
			const std::string& panelName , 
			FrameInfo&         frameInfo
		) 
			: ImguiSlate(panelName, frameInfo) 
		{}

		/**
		* @brief Destructor Function.
		*/
		virtual ~ImguiMaterialPanel() override = default;

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
