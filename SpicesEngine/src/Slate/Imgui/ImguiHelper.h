/**
* @file ImGuiHelper.h
* @brief The ImGuiHelper Class Definitions.
* @author Spices.
*/

#pragma once
#include "Core/Core.h"

// imgui header.
#define IMGUI_DEFINE_MATH_OPERATORS
#include <imgui.h>
//#include <imgui_internal.h>
#include <backends/imgui_impl_vulkan.h>
#include <backends/imgui_impl_glfw.h>

#include <implot.h>
#include <ImGuizmo.h>
#include <IconsMaterialDesign.h>

namespace Spices {

#define ICON_TEXT(icon, text) std::string(icon).append(" ").append(#text).c_str()
#define ICON_EMPTY "     "

	/**
	* @brief Slate Font Mode.
	*/
	enum FontMode
	{
		FONT_FIXED,                    // fixed.
		FONT_PROPORTIONAL_SCALED,      // proportional_scale.
		FONT_MONOSPACED_SCALED,        // monospaced_scale.
		FONT_MAX,                      // max.
	};

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

	/**
	* @brief The ImGuiH Class.
	* This class defines helper function for slate render.
	*/
	class ImGuiH
	{
	public:

		/**
		* @brief Setting common style across samples.
		*/
		static void SetStyle();

		/**
		* @brief Looking for TTF fonts, first on the VULKAN SDK, then Windows default fonts.
		* @param[in] fontmode FontMode.
		*/
		static void SetFonts(FontMode fontmode = FontMode::FONT_PROPORTIONAL_SCALED);

		/**
		* @brief Begin a docking space.
		* @param[in] side Usually Side::Scene.
		* @param[in] alpha Opacity.
		*/
		static void MainDockSpace(Side side = Side::Scene, float alpha = 1.0f);

		/**
		* @brief Get Line Width's Square Size.
		* @return Returns the square size of Line width.
		*/
		static ImVec2 GetLineItemSize();

		/**
		* @brief Draw Reset Icon.
		* @param[in] isMove Is Value Moved compare with default.
		* @return Returns true if need reset.
		*/
		static bool DrawResetIcon(const bool& isMove);

		/**
		* @brief ImGuiHelper Style Checkbox.
		* @param[in] isChecked Bool Pointer of is checked or not.
		*/
		static void Checkbox(bool* isChecked);
		
		static void MainMenuTitleSeparator();

		static void DrawPropertyItem(const std::string& itemName, float columeWidth, std::function<void()> func);

	private:

		/**
		* @brief If GLFW has been initialized, returns the DPI scale of the primary monitor. Otherwise, returns 1.
		*/
		static float GetDPIScale();
	};
}

namespace ImGui {

	void CenteredText(const char* label, const ImVec2& size_arg);
}