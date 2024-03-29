#include "pchheader.h"
#include "VulkanShaderModule.h"

#include "Core/FileLibrary.h"

namespace Spiecs {

	VulkanShaderModule::VulkanShaderModule(VulkanState& vulkanState, const std::string& filePath)
		: VulkanObject(vulkanState)
	{
		FileHandle fileHandle = {};
		fileHandle.is_valid = FileLibrary::FileLibrary_Exists(filePath.c_str());

		if (!fileHandle.is_valid)
		{
			throw std::runtime_error("failed to open file!");
		}

		FileLibrary::FileLibrary_Open(filePath.c_str(), FILE_MODE_READ, 1, &fileHandle);

		uint64_t fileSize = 0;
		FileLibrary::FileLibrary_Size(&fileHandle, &fileSize);

		std::vector<char> shaderChar;
		shaderChar.resize(fileSize);
		FileLibrary::FileLibrary_Read_all_bytes(&fileHandle, shaderChar.data(), &fileSize);

		VkShaderModuleCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
		createInfo.codeSize = fileSize;
		createInfo.pCode = reinterpret_cast<const uint32_t*>(shaderChar.data());

		VK_CHECK(vkCreateShaderModule(vulkanState.m_Device, &createInfo, nullptr, &m_ShaderModule));
	}

	VulkanShaderModule::~VulkanShaderModule()
	{
		vkDestroyShaderModule(m_VulkanState.m_Device, m_ShaderModule, nullptr);
	}
}