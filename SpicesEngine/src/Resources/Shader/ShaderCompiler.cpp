/**
* @file ShaderCompiler.cpp.
* @brief The ShaderCompiler Class Implementation.
* @author Spices.
*/

#include "Pchheader.h"
#include "ShaderCompiler.h"
#include "ShaderHelper.h"

namespace Spices {

	void ShaderCompiler::CompileToSPV(const std::string& data, const ShaderStage& stage, const std::string& name, std::vector<uint32_t>& spirv)
	{
		SPICES_PROFILE_ZONE;

		shaderc::Compiler compiler;
		shaderc::CompileOptions options;

		if (true)
		{
			options.SetOptimizationLevel(shaderc_optimization_level_size);
		}

		shaderc::SpvCompilationResult module = compiler.CompileGlslToSpv(data, ShaderHelper::ToShaderCKind(stage), name.c_str(), options);
		if (module.GetCompilationStatus() != shaderc_compilation_status_success) 
		{
			std::stringstream ss;
			ss << "Error compiling module - " << module.GetErrorMessage();

			SPICES_CORE_ERROR(ss.str());
		}

		spirv = { module.cbegin(), module.cend() };
	}
}