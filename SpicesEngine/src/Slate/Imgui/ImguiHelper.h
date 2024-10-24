/**
* @file ImGuiHelper.h
* @brief The ImGuiHelper Class Definitions.
* @author Spices.
*/

#pragma once
#include "Core/Core.h"
#include "Resources/Material/Material.h"

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
#define ICON_TEXT_ROW(icon, text) std::string(icon).append(" ").append(text).c_str()
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
		
		/**
		* @brief Draw main menu titile separator.
		*/
		static void MainMenuTitleSeparator();

		/**
		* @brief Draw a single property.
		* @param[in] itemName Property name.
		* @param[in] columeWidth .
		* @param[in] nameFunc first colume function.
		* @param[in] valFunc second column function.
		*/
		static void DrawPropertyItem(
			const std::string&    itemName    , 
			float                 columeWidth , 
			std::function<void()> nameFunc    , 
			std::function<void()> valFunc
		);

		/**
		* @brief Draw a stylized tree title bar.
		* @param[in] treeName .
		* @param[in] optionFunc Function of draw option.
		* @param[in] treeFunc Function of draw tree.
		*/
		static void DrawTreeTitle(
			const std::string&    treeName   , 
			std::function<void()> optionFunc , 
			std::function<void()> treeFunc
		);

		/**
		* @brief Draw Drag Scale with different p_min p_max.
		* @param[in] label Name.
		* @param[in] data_type ImGuiDataType.
		* @param[in] p_data .
		* @param[in] components .
		* @param[in] v_speed .
		* @param[in] p_min .
		* @param[in] p_max .
		* @param[in] format .
		* @param[in] flags .
		* @return Returns true if draged.
		*/
		static bool S_DragScalarN(
			const char*      label         , 
			ImGuiDataType    data_type      , 
			void*            p_data         , 
			int              components     , 
			float            v_speed = 1.0f ,
			const void*      p_min   = NULL , 
			const void*      p_max   = NULL , 
			const char*      format  = NULL , 
			ImGuiSliderFlags flags   = 0
		);

		/**
		* @brief Draw ConstantParams.
		* @param[in] material .
		* @param[in] data_type ImGuiDataType.
		* @param[in] components Determined by ConstantParams Type(float: 1, float2: 2 ...).
		* @param[in] format float/int.
		* @param[in] value ConstantParams.
		* @return Returns true if value moved.
		*/
		template<typename T>
		static bool DrawMaterialConstParams(
			std::shared_ptr<Material> material   , 
			ImGuiDataType             data_type  , 
			int                       components , 
			const char*               format     ,
			ConstantParams&           value
		);

	private:

		/**
		* @brief Get GLFW DPI Scale.
		* @return If GLFW has been initializedreturns the DPI scale of the primary monitor. Otherwise, returns 1.
		*/
		static float GetDPIScale();
	};

	template<typename T>
	inline bool ImGuiH::DrawMaterialConstParams(
		std::shared_ptr<Material> material   , 
		ImGuiDataType             data_type  , 
		int                       components ,
		const char*               format     ,
		ConstantParams&           value
	)
	{
		SPICES_PROFILE_ZONE;

		T f    = std::any_cast<T>(value.value.paramValue);
		T df   = std::any_cast<T>(value.defaultValue.paramValue);

		T minf = T();
		if (value.hasMinValue) minf = std::any_cast<T>(value.min.paramValue);
		T maxf = T();
		if (value.hasMaxValue) maxf = std::any_cast<T>(value.max.paramValue);

		bool moved = false;
		ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x - ImGuiH::GetLineItemSize().x);
		if (ImGuiH::S_DragScalarN(
			"##", 
			data_type, 
			&f, 
			components, 
			0.01f, 
			(const void*)&minf,
			(const void*)&maxf,
			format
		))
		{
			value.value.paramValue = f;
			material->UpdateMaterial();
			moved = true;
		}

		ImGui::PopItemWidth();
		ImGui::SameLine();
		if (ImGuiH::DrawResetIcon(f != df))
		{
			value.value.paramValue = df;
			material->UpdateMaterial();
			moved = true;
		}

		return moved;
	}
}

namespace ImGui {

	/**
	* @brief Draw a center text.
	* @param[in] label Text.
	* @param[in] size_arg Text Size.
	*/
	void CenteredText(const char* label, const ImVec2& size_arg);
}