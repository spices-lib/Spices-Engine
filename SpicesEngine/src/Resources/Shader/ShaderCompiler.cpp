/**
* @file ShaderCompiler.cpp.
* @brief The ShaderCompiler Class Implementation.
* @author Spices.
*/

#include "Pchheader.h"
#include "ShaderCompiler.h"
#include "ShaderHelper.h"

#include <glslc/src/file_includer.h>
#include <libshaderc_util/include/libshaderc_util/file_finder.h>

namespace Spices {

	void ShaderCompiler::CompileToSPV(const std::string& data, const ShaderStage& stage, const std::string& name, std::vector<uint8_t>& spirv)
	{
		SPICES_PROFILE_ZONE;

		/**
		* @brief Instance Compiler.
		*/
		shaderc::Compiler compiler;
		shaderc::CompileOptions options;

		/**
		* @brief Shader Header Search folder and processer.
		*/
		shaderc_util::FileFinder fileFinder;
		fileFinder.search_path().push_back(SPICES_ENGINE_ASSETS_PATH + "Shaders/src");

#ifdef SPICES_DEBUG

		options.SetGenerateDebugInfo();                                                           // -g

#endif

		/**
		* @brief Compile Settings.
		*/
		options.SetOptimizationLevel(shaderc_optimization_level_performance);                     // -o                                                
		options.SetTargetSpirv(shaderc_spirv_version_1_6);                                        // --target-env=spv1.6
		options.SetTargetEnvironment(shaderc_target_env_vulkan, shaderc_env_version_vulkan_1_3);  // --target-env=vulkan1.3
		options.SetIncluder(std::make_unique<glslc::FileIncluder>(&fileFinder));

		/**
		* @brief Runtime Compile.
		*/
		shaderc::SpvCompilationResult module = compiler.CompileGlslToSpv(data, ShaderHelper::ToShaderCKind(stage), name.c_str(), options);
		if (module.GetCompilationStatus() != shaderc_compilation_status_success) 
		{
			std::stringstream ss;
			ss << "Error compiling module - " << module.GetErrorMessage();

			SPICES_CORE_ERROR(ss.str());
		}
		
		/**
		* @brief Get Compiled spirv code.
		*/
		std::vector<uint32_t> code32 = { module.cbegin(), module.cend() };
		spirv.resize(code32.size() * 4);
		memcpy(spirv.data(), code32.data(), spirv.size());
	}
}