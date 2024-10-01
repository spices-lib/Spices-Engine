/**
* @file ImguiGPUMemoryHUD.h
* @brief The ImguiGPUMemoryHUD Class Definitions.
* @author Spices.
*/

#pragma once
#include "Core/Core.h"
#include "Slate/Imgui/ImguiUtils.h"

namespace Spices {

	/*
	* @brief The ImguiGPUMemoryHUD Class.
	* This class defines how to render GPU Memory Statics HUD.
	*/
	class ImguiGPUMemoryHUD : public ImguiSlate
	{
	public:

		/**
		* @brief Constructor Function.
		* @param[in] panelName The Slate's name to show.
		* @param[in] frameInfo The Frame Date that in use.
		*/
		ImguiGPUMemoryHUD(
			const std::string& panelName ,
			FrameInfo&         frameInfo
		);

		/**
		* @brief Destructor Function.
		*/
		virtual ~ImguiGPUMemoryHUD() override = default;

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
		* @brief This interface is called on global event function pointer execute.
		* @param[in] event Event.
		*/
		virtual void OnEvent(Event& event) override {};

	private:

		/**
		* @brief Conbert bytes to string(GB/MB/KB).
		* @param[in] bytes .
		* @return Returns string.
		*/
		std::string ConvertBytestoString(uint64_t bytes);

		/**
		* @brief Conbert memory flags to string.
		* @param[in] flag VkMemoryHeapFlags.
		* @return Returns string.
		*/
		std::string ConvertMemoryFlagtoString(VkMemoryHeapFlags flag);

	};
}