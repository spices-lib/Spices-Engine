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
#include "Render/FrameInfo.h"

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
		ImguiSlate(
			const std::string& panelName, 
			FrameInfo&         frameInfo
		) 
			: m_PanelName(panelName)
			, m_FrameInfo(frameInfo) 
		{};

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

		/**
		* @brief Begin a common slate with name.
		* @param[in] panelName The name of Slate.
		* @param[in] alpha Opacity.
		*/
		void Begin(const std::string& panelName, float alpha = 0.5f);

		/**
		* @brief End a slate.
		*/
		void End();

		/**
		* @brief Load a Texture from ResourcePool.
		* @param[out] id The ImTextureID Created with inconFilePath.
		* @param[in] iconFile The file path of icon.
		*/
		void LoadSlateIcon(ImTextureID& id, const std::string& iconFile);

	public:

		/**
		* @brief Set Slate is rendererable or not.
		* @param[in] isSlateOn The boolean of renderable.
		*/
		void SetWindowState(bool isSlateOn) { m_IsSlateOn = isSlateOn; };

	public:

		/**
		* @brief Get Panel Size.
		* @return Returns the Panel Size.
		*/
		inline ImVec2& GetPanelSize() { return m_PanelSize; };

		/**
		* @brief Get Panel Pos.
		* @return Returns the Panel Pos.
		*/
		inline ImVec2& GetPanelPos() { return m_PanelPos; };

		/**
		* @brief Get is IsFocused.
		* @return Returns true if focused.
		*/
		inline bool IsFocused() { return m_IsFocused; };

		/**
		* @brief Get is IsHovered.
		* @return  Returns true if hovered.
		*/
		inline bool IsHovered() { return m_IsHovered; };

	protected:

		/**
		* @brief Main docking slate ID.
		*/
		static ImGuiID dockspaceID;

		/**
		* @brief This slate's size.
		*/
		ImVec2 m_PanelSize = { 1920, 1058 };

		/**
		* @brief This slate's position in desktop.
		*/
		ImVec2 m_PanelPos;

		/**
		* @brief This slate's name.
		*/
		std::string m_PanelName;

		/**
		* @brief The FrameData reference.
		*/
		FrameInfo& m_FrameInfo;

		/**
		* @brief turn false if click quit botton. 
		*/
		bool m_IsOpen = true;

		/**
		* @brief True if Focuse.
		*/
		bool m_IsFocused = false;

		/**
		* @brief True if Hovered.
		*/
		bool m_IsHovered = false;

		/**
		* @breif The boolean of rendererable.
		*/
		bool m_IsSlateOn = false;
	};
}