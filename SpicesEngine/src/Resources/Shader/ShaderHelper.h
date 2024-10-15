/**
* @file ShaderHelper.h.
* @brief The ShaderHelper Class Definitions.
* @author Spices.
*/

#pragma once
#include "Core/Core.h"
#include "shaderc/shaderc.hpp"

namespace Spices {

	/**
	* @brief enum of shader stage.
	*/
	enum class ShaderStage
	{
		vert = 0,          // Vertex Shader.
		geom = 1,          // Geometry Shader.
		tesc = 2,          // Tessellation Control Shader.
		tese = 3,          // Tessellation Evaluation Shader.
		frag = 4,          // Fragment Shader.

		task = 5,          // Task Shader.
		mesh = 6,          // Mesh Shader.

		rgen = 7,          // Ray Generation Shader.
		rint = 8,          // Ray Intersection Shader.
		rchit = 9,         // Ray Closest Hit Shader.
		rahit = 10,        // Ray Any Hit Shader.
		rmiss = 11,        // Ray Any Missing Shader.

		comp = 12,         // Compute Shader.
		glsl = 13,         // GLSL Shader.
	};

	/**
	* @brief ShaderHelper Class.
	*/
	class ShaderHelper
	{
	public:

		/**
		* @brief Convert ShaderStage to String.
		* @param[in] stage ShaderStage.
		* @return Returns String.
		*/ 
		static std::string ToString(ShaderStage stage);

		/**
		* @brief Convert String to ShaderStage.
		* @param[in] stage String.
		* @return Returns ShaderStage.
		*/ 
		static ShaderStage ToStage(std::string stage);

		/**
		* @brief Convert ShaderStage to shaderc_shader_kind.
		* @param[in] stage ShaderStage.
		* @return Returns shaderc_shader_kind.
		*/
		static shaderc_shader_kind ToShaderCKind(ShaderStage stage);

		static VkShaderStageFlagBits ToFlagBits(ShaderStage stage);
	};

}