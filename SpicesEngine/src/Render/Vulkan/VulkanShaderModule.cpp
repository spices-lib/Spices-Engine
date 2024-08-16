/**
* @file VulkanShaderModule.cpp.
* @brief The VulkanShaderModule Class Implementation.
* @author Spices.
*/

#include "Pchheader.h"
#include "VulkanShaderModule.h"

#include "Core/Library/FileLibrary.h"

namespace Spices {

	VulkanShaderModule::VulkanShaderModule(
		VulkanState&       vulkanState , 
		const std::string& shaderName  ,
		const std::string& shaderStage
	)
		: VulkanObject(vulkanState)
		, m_ShaderStage(shaderStage)
	{
		SPICES_PROFILE_ZONE;

		/**
		* @brief Get Shader File Path.
		*/
		const std::string filePath = GetShaderPath(shaderName, shaderStage);

		/**
		* @brief Confirm file existence.
		*/
		FileHandle fileHandle = {};
		fileHandle.is_valid = FileLibrary::FileLibrary_Exists(filePath.c_str());

		if (!fileHandle.is_valid)
		{
			SPICES_CORE_ERROR("File is not exist.");
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
		VK_CHECK(vkCreateShaderModule(vulkanState.m_Device, &createInfo, nullptr, &m_ShaderModule))
		VulkanDebugUtils::SetObjectName(VK_OBJECT_TYPE_SHADER_MODULE, (uint64_t)m_ShaderModule, m_VulkanState.m_Device, shaderName);

		FileLibrary::FileLibrary_Close(&fileHandle);
	}

	VulkanShaderModule::~VulkanShaderModule()
	{
		SPICES_PROFILE_ZONE;

		/**
		* @brief Destroy Shader Module.
		*/
		vkDestroyShaderModule(m_VulkanState.m_Device, m_ShaderModule, nullptr);
	}

	VkPipelineShaderStageCreateInfo VulkanShaderModule::GetShaderStageCreateInfo() const
	{
		VkPipelineShaderStageCreateInfo shaderStages{};

		shaderStages.sType                                      = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		shaderStages.module                                     = m_ShaderModule;
		shaderStages.pName                                      = "main";
		shaderStages.flags                                      = 0;
		shaderStages.pNext                                      = nullptr;
		shaderStages.pSpecializationInfo                        = nullptr;
						   
		if     (m_ShaderStage == "vert")  shaderStages.stage    = VK_SHADER_STAGE_VERTEX_BIT;
		else if(m_ShaderStage == "frag")  shaderStages.stage    = VK_SHADER_STAGE_FRAGMENT_BIT;
		else if(m_ShaderStage == "geom")  shaderStages.stage    = VK_SHADER_STAGE_GEOMETRY_BIT;
		else if(m_ShaderStage == "rgen")  shaderStages.stage    = VK_SHADER_STAGE_RAYGEN_BIT_KHR;
		else if(m_ShaderStage == "rmiss") shaderStages.stage    = VK_SHADER_STAGE_MISS_BIT_KHR;
		else if(m_ShaderStage == "rchit") shaderStages.stage    = VK_SHADER_STAGE_CLOSEST_HIT_BIT_KHR;
		else if(m_ShaderStage == "comp")  shaderStages.stage    = VK_SHADER_STAGE_COMPUTE_BIT;
		else if(m_ShaderStage == "mesh")  shaderStages.stage    = VK_SHADER_STAGE_MESH_BIT_EXT;
		else if(m_ShaderStage == "task")  shaderStages.stage    = VK_SHADER_STAGE_TASK_BIT_EXT;
		else
		{
			SPICES_CORE_ERROR("Not Supported Sahder Stage from material.");
		}

		return shaderStages;
	}

	std::string VulkanShaderModule::GetShaderPath(const std::string& name, const std::string& shaderType)
	{
		SPICES_PROFILE_ZONE;

		/**
		* @brief Get full path of shader file.
		*/
		std::stringstream ss;
		ss << SPICES_ENGINE_ASSETS_PATH << "Shaders/spv/Shader." << name << "." << shaderType << ".spv";

		return ss.str();
	}
}