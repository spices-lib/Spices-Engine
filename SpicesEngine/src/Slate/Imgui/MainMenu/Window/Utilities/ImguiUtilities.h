/**
* @file ImguiUtilities.h
* @brief The ImguiUtilities Class Definitions.
* @author Spices.
*/

#pragma once
#include "Core/Core.h"
#include "Slate/Imgui/ImguiUtils.h"

namespace Spices {

	/**
	* @brief Forward Declear.
	*/
	class ImguiGPUProfilerHUD;
	class ImguiGPUMemoryHUD;

	/**
	* @brief The ImguiUtilities Class.
	* This class defines how to render a Utilities.
	*/
	class ImguiUtilities : public ImguiSlate
	{
	public:

		/**
		* @brief Constructor Function.
		* @param[in] panelName The Slate's name to show.
		* @param[in] frameInfo The Frame Date that in use.
		*/
		ImguiUtilities(
			const std::string& panelName,
			FrameInfo& frameInfo
		);

		/**
		* @brief Destructor Function.
		*/
		virtual ~ImguiUtilities() override = default;

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

	private:

		/**
		* @brief The Path of tracy-profiler.
		*/
		std::string m_ProfileProcessName = SPICES_EXTENT_PROCESS_PATH +  "tracy/tracy-profiler.exe";

		/**
		* @brief The shared pointer of ImguiPerfProfiler.
		*/
		std::shared_ptr<ImguiGPUProfilerHUD> m_ImguiGPUProfilerHUD;

		/**
		* @brief The shared pointer of ImguiGPUMemoryHUD.
		*/
		std::shared_ptr<ImguiGPUMemoryHUD> m_ImguiGPUMemoryHUD;
	};
}