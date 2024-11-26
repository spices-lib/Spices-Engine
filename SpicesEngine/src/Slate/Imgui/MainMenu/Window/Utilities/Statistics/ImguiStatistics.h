/**
* @file ImguiStatistics.h
* @brief The ImguiStatistics Class Definitions.
* @author Spices.
*/

#pragma once
#include "Core/Core.h"
#include "Slate/Imgui/ImguiUtils.h"

namespace Spices {

	/**
	* @brief Forward Declare.
	*/
	class ImguiVideoMemoryRuntimeHUD;
	class ImguiGPURuntimeProfilerHUD;
	class ImguiRendererProfilerHUD;

	/*
	* @brief The ImguiStatistics Class.
	* This class defines how to render Debugger Panel.
	*/
	class ImguiStatistics : public ImguiSlate
	{
	public:

		/**
		* @brief Constructor Function.
		* @param[in] panelName The Slate's name to show.
		* @param[in] frameInfo The Frame Date that in use.
		*/
		ImguiStatistics(
			const std::string& panelName,
			FrameInfo& frameInfo
		);

		/**
		* @brief Destructor Function.
		*/
		virtual ~ImguiStatistics() override = default;

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
		* @brief The shared pointer of ImguiVideoMemoryRuntimeHUD.
		*/
		std::shared_ptr<ImguiVideoMemoryRuntimeHUD> m_ImguiVideoMemoryRuntimeHUD;

		/**
		* @brief The shared pointer of ImguiGPURuntimeProfilerHUD.
		*/
		std::shared_ptr<ImguiGPURuntimeProfilerHUD> m_ImguiGPURuntimeProfilerHUD;

		/**
		* @brief The shared pointer of ImguiRendererProfilerHUD.
		*/
		std::shared_ptr<ImguiRendererProfilerHUD> m_ImguiRendererProfilerHUD;
	};
}