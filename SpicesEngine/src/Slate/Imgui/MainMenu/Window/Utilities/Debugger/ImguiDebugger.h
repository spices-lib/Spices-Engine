/**
* @file ImguiDebugger.h
* @brief The ImguiDebugger Class Definitions.
* @author Spices.
*/

#pragma once
#include "Core/Core.h"
#include "Slate/Imgui/ImguiUtils.h"

namespace Spices {

	/**
	* @brief Forward Declear.
	*/
	class ImguiVideoMemoryRuntimeHUD;
	class ImguiGPURuntimeProfilerHUD;

	/*
	* @brief The ImguiDebugger Class.
	* This class defines how to render Debugger Panel.
	*/
	class ImguiDebugger : public ImguiSlate
	{
	public:

		/**
		* @brief Constructor Function.
		* @param[in] panelName The Slate's name to show.
		* @param[in] frameInfo The Frame Date that in use.
		*/
		ImguiDebugger(
			const std::string& panelName,
			FrameInfo& frameInfo
		);

		/**
		* @brief Destructor Function.
		*/
		virtual ~ImguiDebugger() override = default;

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
		* @brief The Path of tracy-profiler.
		*/
		std::string m_ProfileProcessName = SPICES_EXTENT_PROCESS_PATH + "tracy/tracy-profiler.exe";

		/**
		* @brief The shared pointer of ImguiVideoMemoryRuntimeHUD.
		*/
		std::shared_ptr<ImguiVideoMemoryRuntimeHUD> m_ImguiVideoMemoryRuntimeHUD;

		/**
		* @brief The shared pointer of ImguiGPURuntimeProfilerHUD.
		*/
		std::shared_ptr<ImguiGPURuntimeProfilerHUD> m_ImguiGPURuntimeProfilerHUD;
	};
}