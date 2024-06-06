/**
* @file VulkanShaderModule.cpp.
* @brief The VulkanShaderModule Class Implementation.
* @author Spiecs.
*/

#include "Pchheader.h"
#include "VulkanShaderModule.h"

#include "Core/Library/FileLibrary.h"

namespace Spiecs {

	VulkanShaderModule::VulkanShaderModule(
		VulkanState&       vulkanState , 
		const std::string& shaderName  ,
		const std::string& shaderStage
	)
		: VulkanObject(vulkanState)
	{
		SPIECS_PROFILE_ZONE;

		/**
		* @brief Get Shader File Path.
		*/
		std::string filePath = GetSahderPath(shaderName, shaderStage);

		/**
		* @brief Confirm file existence.
		*/
		FileHandle fileHandle = {};
		fileHandle.is_valid = FileLibrary::FileLibrary_Exists(filePath.c_str());

		if (!fileHandle.is_valid)
		{
			SPIECS_CORE_ERROR("File is not exist.");
		}

		/**
		* @brief Open Shader File.
		*/
		FileLibrary::FileLibrary_Open(filePath.c_str(), FILE_MODE_READ, 1, &fileHandle);

		uint64_t fileSize = 0;
		FileLibrary::FileLibrary_Size(&fileHandle, &fileSize);

		/**
		* @brief Read Shader File.
		*/
		std::vector<char> shaderChar;
		shaderChar.resize(fileSize);
		FileLibrary::FileLibrary_Read_all_bytes(&fileHandle, shaderChar.data(), &fileSize);

		/**
		* @brief Instance a VkShaderModuleCreateInfo.
		*/
		VkShaderModuleCreateInfo createInfo{};
		createInfo.sType    = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
		createInfo.codeSize = fileSize;
		createInfo.pCode    = reinterpret_cast<const uint32_t*>(shaderChar.data());

		/**
		* @brief Create Shader Module.
		*/
		VK_CHECK(vkCreateShaderModule(vulkanState.m_Device, &createInfo, nullptr, &m_ShaderModule));
		VulkanDebugUtils::SetObjectName(VK_OBJECT_TYPE_SHADER_MODULE, m_ShaderModule, m_VulkanState.m_Device, shaderName);
	}

	VulkanShaderModule::~VulkanShaderModule()
	{
		SPIECS_PROFILE_ZONE;

		/**
		* @brief Destroy Shader Module.
		*/
		vkDestroyShaderModule(m_VulkanState.m_Device, m_ShaderModule, nullptr);
	}

	std::string VulkanShaderModule::GetSahderPath(const std::string& name, const std::string& shaderType)
	{
		SPIECS_PROFILE_ZONE;

		/**
		* @brief Get full path of shader file.
		*/
		std::stringstream ss;
		ss << SPIECS_ENGINE_ASSETS_PATH << "Shaders/spv/Shader." << name << "." << shaderType << ".spv";

		return ss.str();
	}
}