/**
* @file ImguiViewport.h
* @brief The ImguiViewport Class Definitions.
* @author Spices.
*/

#pragma once
#include "Core/Core.h"
#include "Slate/Imgui/ImguiUtils.h"

namespace Spices {

	/**
	* @brief Forward Declear.
	*/
	class ImguiFloattingInfo;

	/**
	* @brief Forward Declear.
	*/
	class ImguiGizmos;

	/**
	* @brief Forward Declear.
	*/
	class ImguiViewportToolBar;

	/**
	* @brief The ImguiViewport Class.
	* This class defines how to render a viewport.
	*/
	class ImguiViewport : public ImguiSlate
	{
	public:

		/**
		* @brief Constructor Function.
		* @param[in] panelName The Slate's name to show.
		* @param[in] frameInfo The Frame Date that in use.
		*/
		ImguiViewport(
			const std::string& panelName , 
			FrameInfo&         frameInfo
		);

		/**
		* @brief Destructor Function.
		*/
		virtual ~ImguiViewport() {};

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
		virtual void OnEvent(Event& event) override;

	protected:

		/**
		* @brief Query whether viewport is resized this frame.
		* Clamp min m_panelsize value to 1 here, for vulkan can not work with 0 extent.
		* @param[in] thisFrameSize The size of Viewport this Frame.
		*/
		virtual void QueryIsResizedThisFrame(const ImVec2& thisFrameSize) override;

	public:

		/**
		* @brief Get Mouse Position Relative Viewport(Might be negative value).
		* @return Returns the pair of PostionX, PostionY.
		*/
		std::pair<uint32_t, uint32_t> GetMousePosInViewport();

		/**
		* @brief Get Gizmo Shared Pointer.
		* @return Returns the Gizmo Shared Pointer.
		*/
		inline std::shared_ptr<ImguiGizmos> GetGizmo() { return m_Gizmos; };

	private:

		/**
		* @brief Event Dispatcher target. Registy on Slate(Viewport) Resized.
		* @param[in] event SlateResizeEvent.
		* @return Returns true if need block the event.
		*/
		bool OnSlateResize(SlateResizeEvent& event);

		/**
		* @brief Event Dispatcher target. Registy on Windows Resized.
		* @param[in] event WindowResizeOverEvent.
		* @return Returns true if need block the event.
		*/
		bool OnWindowResizeOver(WindowResizeOverEvent& event);

	private:

		/**
		* @brief ImTextureID of SceneColor.
		*/
		ImTextureID m_ViewportID;

		/**
		* @brief The shared pointer of ImguiFloattingInfo.
		*/
		std::shared_ptr<ImguiFloattingInfo> m_FloattingInfo;

		/**
		* @brief The shared pointer of ImguiGizmos.
		*/
		std::shared_ptr<ImguiGizmos> m_Gizmos;

		/**
		* @brief The shared pointer of ImguiGizmos.
		*/
		std::shared_ptr<ImguiViewportToolBar> m_ToolBar;
	};
}