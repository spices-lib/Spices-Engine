/**
* @file VulkanShaderModule.h.
* @brief The VulkanShaderModule Class Definitions.
* @author Spices.
*/

#pragma once
#include "Core/Core.h"
#include "VulkanUtils.h"
#include "Resources/Shader/ShaderHelper.h"

namespace Spices {

	/**
	* @brief This Class is a Wrapper of VkShaderModule.
	*/
	class VulkanShaderModule : public VulkanObject
	{
	public:

		/**
		* @brief Constructor Function.
		* Create VkShaderModule.
		* @param[in] vulkanState The global VulkanState.
		* @param[in] shaderName The Shader name.
		* @param[in] shaderStage The Shader usage stage.
		*/
		VulkanShaderModule(
			VulkanState&       vulkanState , 
			const std::string& shaderName  , 
			const std::string& shaderStage
		);

		/**
		* @brief Constructor Function.
		* Create VkShaderModule.
		* @param[in] vulkanState The global VulkanState.
		* @param[in] shaderName The Shader name.
		* @param[in] shaderStage The Shader usage stage.
		* @param[in] spirv Spirv Code.
		* @param[in] fullPath Shader full Path.
		*/
		VulkanShaderModule(
			VulkanState&                 vulkanState , 
			const std::string&           shaderName  , 
			const ShaderStage&           shaderStage ,
			const std::vector<uint32_t>& spirv       ,
			const std::string&           fullPath
		);

		/**
		* @brief Destructor Function.
		*/
		virtual ~VulkanShaderModule() override;

		/**
		* @brief Get VkShaderModule this class handled.
		* @return Returns the VkShaderModule this class handled.
		*/
		VkShaderModule& Get() { return m_ShaderModule; }


		VkPipelineShaderStageCreateInfo GetShaderStageCreateInfo() const;

	private:

		/**
		* @brief Get shader path string.
		* @param[in] name What shader name is.
		* @param[in] shaderType What shader type is.
		* @return Returns the shader path string.
		*/
		std::string GetShaderPath(
			const std::string& name       , 
			const std::string& shaderType
		);

	private:

		/**
		* @brief The VkShaderModule this class handled.
		*/
		VkShaderModule m_ShaderModule;

		/**
		*brief Shader Stage.
		*/
		ShaderStage m_ShaderStage;
	};
}