/**
* @file ImGuiHelper.h
* @brief The ImGuiHelper Class Definitions.
* @author Spiecs.
*/

#pragma once
#include "Core/Core.h"

// imgui header.
#define IMGUI_DEFINE_MATH_OPERATORS
#include <imgui.h>
#include <backends/imgui_impl_vulkan.h>
#include <backends/imgui_impl_glfw.h>
#include <implot.h>
#include <ImGuizmo.h>

namespace Spiecs {

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

	private:

		/**
		* @brief If GLFW has been initialized, returns the DPI scale of the primary monitor. Otherwise, returns 1.
		*/
		static float GetDPIScale();
	};
}