/**
* @file ShaderManager.h
* @brief The ShaderManager Class Definitions.
* @author Spices.
*/

#pragma once
#include "Core/Core.h"
#include "Render/Vulkan/VulkanShaderModule.h"
#include "Core/Math/Math.h"

namespace Spices {

	using ShaderManagerContainer = std::unordered_map<String2, std::shared_ptr<VulkanShaderModule>>;

	/**
	* @brief This Class manages all shaders this project.
	*/
	class ShaderManager
	{
	public:
		/**
		* @brief Constructor Function.
		*/
		ShaderManager() = default;

		/**
		* @brief Destructor Function.
		*/
		virtual ~ShaderManager() = default;

		/**
		* @brief Registry a VulkanShaderModule, create one if find none.
		* @param name The shader's name.
		* @param stage The shader's stage.
		* @return The shared pointer reference of VulkanShaderModule.
		*/
		static std::shared_ptr<VulkanShaderModule>& Registry(const std::string& name, const std::string& stage);

	private:

		/**
		* @brief The container of all shaders, identified with shader's name and stage.
		*/
		static ShaderManagerContainer m_Shaders;
	};

}