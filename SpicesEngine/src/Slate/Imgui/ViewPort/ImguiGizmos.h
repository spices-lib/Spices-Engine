/**
* @file ImguiGizmos.h
* @brief The ImguiGizmos Class Definitions.
* @author Spices.
*/

#pragma once
#include "Core/Core.h"
#include "Slate/Imgui/ImguiUtils.h"

namespace Spices {

	/**
	* @brief The ImguiGizmos Class.
	* This class defines how to render a viewport gizmo.
	*/
	class ImguiGizmos : public ImguiSlate
	{
	public:

		/**
		* @brief Constructor Function.
		* @param[in] panelName The Slate's name to show.
		* @param[in] frameInfo The Frame Date that in use.
		* @param[in] owner The slate Owner.
		*/
		ImguiGizmos(
			const std::string& panelName ,
			FrameInfo&         frameInfo ,
			ImguiSlate*        owner
		) 
			: ImguiSlate(panelName, frameInfo) 
			, m_Owner(owner)
		{};

		/**
		* @brief Destructor Function.
		*/
		virtual ~ImguiGizmos() {};

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

	public:

		/**
		* @brief Is mouse is over gizmo.
		* @return Returns true if it does.
		*/
		bool IsOver() { return ImGuizmo::IsOver(); };

	private:

		/**
		* @brief Event Dispatcher target. Registy on Key Resized.
		* @param[in] e KeyPressedEvent.
		* @return Returns true if need block the event.
		*/
		bool OnKeyPressed(KeyPressedEvent& e);

	private:

		/**
		* @brief The owner, usually is a ImguiViewport class.
		*/
		ImguiSlate* m_Owner;

		/**
		* @brief Gizmo Type, Transpose, rotate, scale, universal
		*/
		int m_GizmoType = -1;

		/**
		* @brief Is Enable Gizmo.
		*/
		bool bEnableGizmo = false;
	};
}