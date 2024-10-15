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
		* @brief Destructor Function.
		*/
		virtual ~Shader();

	private:

		std::string m_ShaderName;
		ShaderStage m_ShaderStage;
		std::shared_ptr<VulkanShaderModule> m_ShaderModule;

		friend class ShaderLoader;
	};

}