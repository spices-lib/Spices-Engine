/**
* @file VulkanShaderModule.h.
* @brief The VulkanShaderModule Class Definitions.
* @author Spiecs.
*/

#pragma once
#include "Core/Core.h"
#include "VulkanUtils.h"

namespace Spiecs {

	/**
	* @brief This Class is a Wapper of VkShaderModule.
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
		* @brief Destructor Function.
		*/
		virtual ~VulkanShaderModule();

		/**
		* @brief Get VkShaderModule this class handled.
		* @return Returns the VkShaderModule this class handled.
		*/
		VkShaderModule& Get() { return m_ShaderModule; };

	private:

		/**
		* @brief Get sahder path string.
		* @param[in] name What shader name is.
		* @param[in] shaderType What shader type is.
		* @return Returns the sahder path string.
		*/
		std::string GetSahderPath(
			const std::string& name       , 
			const std::string& shaderType
		);

	private:

		/**
		* @brief The VkShaderModule this class handled.
		*/
		VkShaderModule m_ShaderModule;
	};
}