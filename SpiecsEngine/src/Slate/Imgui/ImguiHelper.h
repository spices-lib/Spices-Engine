#pragma once
#include "Core/Core.h"

#define IMGUI_DEFINE_MATH_OPERATORS
#include <imgui.h>
#include <backends/imgui_impl_vulkan.h>
#include <backends/imgui_impl_glfw.h>

namespace Spiecs {

	enum FontMode
	{
		FONT_FIXED,
		FONT_PROPORTIONAL_SCALED,
		FONT_MONOSPACED_SCALED,
	};

	class ImGuiH
	{

	public:

		/**
		* @brief Setting common style across samples.
		*/
		static void SetStyle();

		/**
		* @brief Looking for TTF fonts, first on the VULKAN SDK, then Windows default fonts.
		*/
		static void SetFonts(FontMode fontmode = FontMode::FONT_PROPORTIONAL_SCALED);

	private:

		/**
		* @brief If GLFW has been initialized, returns the DPI scale of the primary monitor. Otherwise, returns 1.
		*/
		static float GetDPIScale();
	};
}