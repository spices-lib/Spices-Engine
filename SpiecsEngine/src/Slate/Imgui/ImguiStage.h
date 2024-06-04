/**
* @file ImguiStage.h
* @brief The ImguiStage Class Definitions.
* @author Spiecs.
*/

#pragma once
#include "Core/Core.h"
#include "ImguiUtils.h"

namespace Spiecs {

	/**
	* @brief The ImguiStage Class.
	* This class defines how to render a Stage.
	*/
	class ImguiStage : public ImguiSlate
	{
	public:

		/**
		* @brief Constructor Function.
		* @param[in] panelName The Slate's name to show.
		* @param[in] frameInfo The Frame Date that in use.
		*/
		ImguiStage(
			const std::string& panelName  , 
			FrameInfo&         frameInfo
		);

		/**
		* @brief Destructor Function.
		*/
		virtual ~ImguiStage() {};

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
	};
}