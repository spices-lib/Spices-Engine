/**
* @file ShaderLoader.cpp.
* @brief The ShaderLoader Class Implementation.
* @author Spices.
*/

#include "Pchheader.h"
#include "ShaderLoader.h"
#include "Systems/ResourceSystem.h"
#include "Core/Library/FileLibrary.h"
#include "Resources/Shader/Shader.h"
#include "Resources/Shader/ShaderCompiler.h"
#include "Render/Vulkan/VulkanRenderBackend.h"
#include "Render/Vulkan/VulkanShaderModule.h"
#include "shaderc/shaderc.hpp"

namespace Spices {

	/**
	* @brief Const variable: Original Shader File Path.
	*/
	const std::string defaultShaderPath = "Shaders/src/";

	bool ShaderLoader::Load(const std::string& fileName, ShaderStage stage, Shader* outShader)
	{
		SPICES_PROFILE_ZONE;

		bool isFind = false;
		std::string filePath;
		for (auto& it : ResourceSystem::GetSearchFolder())
		{
			filePath = it + defaultShaderPath + "Shader." + fileName + "." + ShaderHelper::ToString(stage);
			if (FileLibrary::FileLibrary_Exists(filePath.c_str()))
			{
				isFind = true;
				break;
			}
		}
		if (!isFind)
		{
			std::stringstream ss;
			ss << "Shader: " << fileName << "file is not finded";

			SPICES_CORE_WARN(ss.str());

			return false;
		}

		/**
		* @brief Read file as bytes.
		*/
		std::ifstream stream(filePath);
		std::stringstream strStream;
		strStream << stream.rdbuf();

		/**
		* @brief Compile to spv.
		*/
		std::vector<uint32_t> spirv;
		ShaderCompiler::CompileToSPV(strStream.str(), stage, fileName, spirv);
		
		/**
		* @brief Create shader module.
		*/
		outShader->m_ShaderModule = std::make_shared<VulkanShaderModule>(VulkanRenderBackend::GetState(), fileName, stage, spirv, filePath);

		return true;
	}
}