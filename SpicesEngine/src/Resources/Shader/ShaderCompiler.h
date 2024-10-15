/**
* @file ShaderCompiler.h.
* @brief The ShaderCompiler Class Definitions.
* @author Spices.
*/

#pragma once
#include "Core/Core.h"
#include "ShaderHelper.h"

namespace Spices {

	/**
	* @brief Wapper of shaderc compiler.
	*/
	class ShaderCompiler
	{
	public:

		/**
		* @brief Compile glsl to spv.
		* @param[in] data glsl data.
		* @param[in] stage ShaderStage.
		* @param[in] name Shader Name.
		* @param[in,out] spirv spv data.
		*/
		static void CompileToSPV(const std::string& data, const ShaderStage& stage, const std::string& name, std::vector<uint32_t>& spirv);
	};

}