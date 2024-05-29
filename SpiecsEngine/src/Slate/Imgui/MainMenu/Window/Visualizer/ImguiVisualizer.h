/**
* @file ImguiVisualizer.h
* @brief The ImguiVisualizer Class Definitions.
* @author Spiecs.
*/

#pragma once
#include "Core/Core.h"
#include "Slate/Imgui/ImguiUtils.h"

namespace Spiecs {

	/**
	* @brief Forward Declear.
	*/
	class ImguiGBufferVisualizer;

	/**
	* @brief The ImguiVisualizer Class.
	* This class defines how to render a Visualizer.
	*/
	class ImguiVisualizer : public ImguiSlate
	{
	public:

		/**
		* @brief Constructor Function.
		* @param[in] panelName The Slate's name to show.
		* @param[in] frameInfo The Frame Date that in use.
		*/
		ImguiVisualizer(
			const std::string& panelName , 
			FrameInfo&         frameInfo
		);

		/**
		* @brief Destructor Function.
		*/
		virtual ~ImguiVisualizer() {};

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
		* @brief The shared pointer of ImguiGBufferVisualizer.
		*/
		std::shared_ptr<ImguiGBufferVisualizer> m_GBufferVisualizer;
	};
}