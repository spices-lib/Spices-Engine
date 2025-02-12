/**
* @file Shader.h.
* @brief The Shader Class Definitions.
* @author Spices.
*/

#pragma once
#include "Core/Core.h"
#include "Resources/Loader/ShaderLoader.h"
#include "ShaderHelper.h"
#include "Render/Vulkan/VulkanShaderModule.h"

namespace Spices {

	/**
	* @brief Shader Class.
	*/
	class Shader
	{
	public:

		/**
		* @brief Constructor Function.
		*/
		Shader() = default;

		/**
		* @brief Constructor Function.
		* @param[in] name Shader Name.
		* @param[in] stage ShaderStage.
		*/
		Shader(const std::string& name, ShaderStage stage);
		
		/**
		* @brief Constructor Function.
		* @param[in] name Shader Name.
		* @param[in] stage ShaderStage.
		*/
		Shader(const std::string& name, const std::string& stage);

		/**
		* @brief Destructor Function.
		*/
		virtual ~Shader() = default;

		/**
		* @brief Deserialize the data from a disk file to this class.
		*/
		void Deserialize();

		/**
		* @brief Get VulkanShaderModule.
		* @return Returns VulkanShaderModule.
		*/
		std::shared_ptr<VulkanShaderModule>& GetShaderModule() { return m_ShaderModule; }

	private:

		/**
		* @brief Shader name.
		*/
		std::string m_ShaderName;

		/**
		* @brief Shader stage.
		*/
		ShaderStage m_ShaderStage;

		/**
		* @brief VulkanShaderModule.
		*/
		std::shared_ptr<VulkanShaderModule> m_ShaderModule;

		friend class ShaderLoader;
	};

}