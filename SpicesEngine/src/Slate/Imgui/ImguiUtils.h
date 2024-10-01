/**
* @file ImguiSlate.h.
* @brief The ImguiSlate Class Implementation.
* @author Spices.
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

namespace Spices{

	/**
	* @brief This Class defines the basic behaver of specific slate.
	* When we add an new Slate, we need inherit from this.
	*/
	class ImguiSlate
	{
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
		* @brief Begin a common slate.
		* @param[in] alpha Opacity.
		* @param[in] flags Slate flags.
		*/
		void Begin(float alpha = 1.0f, ImGuiWindowFlags flags = 0);

		/**
		* @brief Begin a common slate with name.
		* @param[in] panelName The name of Slate.
		* @param[in] alpha Opacity.
		* @param[in] flags Slate flags.
		*/
		void Begin(const std::string& panelName, float alpha = 1.0f, ImGuiWindowFlags flags = 0);

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

		/**
		* @brief Query whether viewport is resized this frame.
		* @param[in] thisFrameSize The size of Viewport this Frame.
		*/
		virtual void QueryIsResizedThisFrame(const ImVec2& thisFrameSize);

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

		/**
		* @brief Get boolean of whether resized this frame.
		* @return Returns true if resized.
		*/
		inline bool IsResizedThisFrame() { return m_IsResized; };

	protected:

		/**
		* @brief This slate's size.
		*/
		ImVec2 m_PanelSize = { 1, 1 };

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

		/**
		* @brief Boolean of whether resized this frame.
		*/
		bool m_IsResized = false;
	};
}