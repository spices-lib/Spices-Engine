/**
* @file ImguiSlate.h.
* @brief The ImguiSlate Class Implementation.
* @author Spiecs.
*/

#pragma once
#include "Core/Core.h"

// Event Header.
#include "Core/Event/KeyEvent.h"
#include "Core/Event/MouseEvent.h"
#include "Core/Event/WindowEvent.h"
#include "Core/Event/SlateEvent.h"

// Helper Function Header.
#include "ImguiHelper.h"

namespace Spiecs{

	/**
	* @brief This Class defines the basic behaver of specific slate.
	* When we add an new Slate, we need inherit from this.
	*/
	class ImguiSlate
	{
	public:

		/**
		* @brief Where the slate's initialized pos.
		*/
		enum class Side
		{
			Left,    // The Left          on the window.
			Right,   // The Right         on the window.
			Up,      // The Up            on the window.
			Down,    // The Down          on the window.
			Scene,   // The full screen   on the window.
		};

	public:

		/**
		* @brief Constructor Function.
		* Init with Slate's name.
		* @param[in] panelName The Slate's name.
		*/
		ImguiSlate(const std::string& panelName) : m_PanelName(panelName){};

		/**
		* @brief Destructor Function.
		*/
		virtual ~ImguiSlate() {};

		/**
		* @brief This interface is called on SlateSystem Update.
		* @param[in] ts TimeStep.
		*/
		virtual void OnUpdate(TimeStep& ts) = 0;

		/**
		* @brief This interface is called on SlateRenderer Render.
		*/
		virtual void OnRender() = 0;

		/**
		* @brief This interface is called on global event function pointer execute.
		* @param[in] event Event.
		*/
		virtual void OnEvent(Event& event) = 0;

	protected:

		/**
		* @brief Begin a docking space, usually called by viewport.
		* @param[in] side Usually Side::Scene.
		* @param[in] alpha Opacity.
		*/
		void BeginMainDock(Side side = Side::Scene, float alpha = 1.0f);

		/**
		* @brief Begin a common slate.
		* @param[in] alpha Opacity.
		*/
		void Begin(float alpha = 0.5f);

		void Begin(const std::string& panelName, float alpha = 0.5f);

		/**
		* @brief End a slate.
		*/
		void End() { ImGui::End(); ImGui::PopStyleVar(3); };

	public:
		void SetWindowState(bool isSlateOn) { m_IsSlateOn = isSlateOn; };

	public:
		ImVec2 GetPanelSize() { return m_PanelSize; };
		ImVec2 GetPanelPos() { return m_PanelPos; };
		bool IsFocused() { return m_IsFocused; };
		bool IsHovered() { return m_IsHovered; };

	protected:

		/**
		* @brief Main docking slate ID.
		*/
		static ImGuiID dockspaceID;

		/**
		* @brief This slate's size.
		*/
		ImVec2 m_PanelSize = { 1920, 1058 };

		ImVec2 m_PanelPos;

		/**
		* @brief This slate's name.
		*/
		std::string m_PanelName;

		/**
		* @brief True if Focuse.
		*/
		bool m_IsFocused = false;

		/**
		* @brief True if Hovered.
		*/
		bool m_IsHovered = false;


		bool m_IsSlateOn = false;
	};
}