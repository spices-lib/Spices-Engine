/**
* @file ImguiViewport.h
* @brief The ImguiViewport Class Definitions.
* @author Spices.
*/

#pragma once
#include "Core/Core.h"
#include "Slate/Imgui/ImguiUtils.h"
#include "Core/Container/BehaveStateList.h"
#include "Slate/SlateImage.h"

namespace Spices {

	/**
	* @brief Forward Declare.
	*/
	class ImguiFloatingInfo;

	/**
	* @brief Forward Declare.
	*/
	class ImguiGizmos;

	/**
	* @brief Forward Declare.
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
		* @param[in] index Viewport Index.
		*/
		ImguiViewport(
			const std::string& panelName , 
			FrameInfo&         frameInfo ,
			uint32_t           index = 0
		);

		/**
		* @brief Destructor Function.
		*/
		virtual ~ImguiViewport() override = default;

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
		* @return Returns the pair of Position X, Position Y.
		*/
		std::pair<uint32_t, uint32_t> GetMousePosInViewport() const;

		/**
		* @brief Get Gizmo Shared Pointer.
		* @return Returns the Gizmo Shared Pointer.
		*/
		inline std::shared_ptr<ImguiGizmos> GetGizmo() { return m_Gizmos; }

		/**
		* @brief Toggle viewport.
		*/
		void Toggle() const;

	private:

		/**
		* @brief Event Dispatcher target. Registry on Slate(Viewport) Resized.
		* @param[in] event SlateResizeEvent.
		* @return Returns true if need block the event.
		*/
		bool OnSlateResize(SlateResizeEvent& event) const;

		/**
		* @brief Event Dispatcher target. Registry on Windows Resized.
		* @param[in] event WindowResizeOverEvent.
		* @return Returns true if need block the event.
		*/
		bool OnWindowResizeOver(WindowResizeOverEvent& event) const;

		/**
		* @brief Toggle focused slate.
		* @param[in] event KeyPressedEvent.
		* @reutrn Returns true if want block the event.
		*/
		bool OnToggleSlate(KeyPressedEvent& event) const;

	private:

		/**
		* @brief SlateImage of SceneColor.
		*/
		std::shared_ptr<SlateImage> m_ViewportContext;

		/**
		* @brief This viewport index.
		*/
		uint32_t m_Index;

		/**
		* @brief The shared pointer of ImguiFloatingInfo.
		*/
		std::shared_ptr<ImguiFloatingInfo> m_FloatingInfo;

		/**
		* @brief The shared pointer of ImguiGizmos.
		*/
		std::shared_ptr<ImguiGizmos> m_Gizmos;

		/**
		* @brief The shared pointer of ImguiGizmos.
		*/
		std::shared_ptr<ImguiViewportToolBar> m_ToolBar;

		/**
		* @brief True if this slate toggled this frame.
		*/
		bool m_IsToggled = false;

		/**
		* @brief Toggle behave state list.
		*/
		std::shared_ptr<scl::behave_state_list<void>> m_ToggleStateList;
	};
}