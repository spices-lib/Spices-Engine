/**
* @file ImguiFloattingInfo.h
* @brief The ImguiFloattingInfo Class Definitions.
* @author Spices.
*/

#pragma once
#include "Core/Core.h"
#include "Slate/Imgui/ImguiUtils.h"

namespace Spices {

	/**
	* @brief The ImguiFloattingInfo Class.
	* This class defines how to render a viewport info.
	*/
	class ImguiFloattingInfo : public ImguiSlate
	{
	public:

		/**
		* @brief Constructor Function.
		* @param[in] panelName The Slate's name to show.
		* @param[in] frameInfo The Frame Date that in use.
		* @param[in] owner The slate Owner.
		*/
		ImguiFloattingInfo(
			const std::string& panelName, 
			FrameInfo&         frameInfo, 
			ImguiSlate*        owner
		) 
			: ImguiSlate(panelName, frameInfo)
			, m_Owner(owner) 
		{};

		/**
		* @brief Destructor Function.
		*/
		virtual ~ImguiFloattingInfo() {};

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
		* @brief The owner, usually is a ImguiViewport class.
		*/
		ImguiSlate* m_Owner;
	};
}