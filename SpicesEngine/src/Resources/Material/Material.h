/**
* @file Material.h.
* @brief The Material Class Definitions.
* @author Spices.
*/

#pragma once
#include "Core/Core.h"
#include "Resources/Texture/Texture2D.h"
#include "Resources/Loader/MaterialLoader.h"
#include "Render/Vulkan/VulkanShaderModule.h"
#include "Resources/Texture/Texture.h"
#include "Core/Math/Math.h"
#include "Core/Container/runtime_memory_block.h"
#include "Core/Container/linked_unordered_map.h"
#include "Render/Vulkan/VulkanBuffer.h"
#include <glm/glm.hpp>
#include <unordered_map>

namespace Spices {

	class VulkanDescriptorSet;

	/**
	* @brief This struct's data is defined from .material file.
	*/
	struct TextureParam
	{
		/**
		* @brief Texture type.
		*/
		std::string textureType;
			
		/**
		* @brief Texture path.
		*/
		std::string texturePath;
			
		/**
		* @brief Which set this texture will use.
		*/
		uint32_t set;

		/**
		* @breif Which binding this texture will use.
		*/
		uint32_t binding;

		/**
		* @breif Which array index this texture will use.
		*/
		uint32_t index;
	};

	/**
	* @brief This struct's data is defined from .material file.
	*/
	struct ConstantParam
	{
		/**
		* @brief Which set this texture will use.
		*/
		uint32_t set;

		/**
		* @breif Which binding this texture will use.
		*/
		uint32_t binding;
			
		/**
		* @brief parameter type.
		*/
		std::string paramType;

		/**
		* @brief parameter value.
		*/
		std::any paramValue;
	};
	
	/**
	* @brief Material Class.
	* This class contains a branch of parameter and shader, also descriptor.
	*/
	class Material
	{
	public:

		struct DescriptorSetBindingInfoHelp
		{
			enum Type
			{
				Buffer,
				Image,
			};
			
			DescriptorSetBindingInfoHelp() = default;
			virtual ~DescriptorSetBindingInfoHelp() = default;
			
			uint32_t count = 0;
			size_t size = 0;
			Type type = Type::Buffer;
		};
		
	public:

		/**
		* @brief Constructor Function.
		*/
		Material() = default;

		/**
		* @brief Constructor Function.
		* Deserialize immediately.
		* Usually call it.
		* @param[in] materialPath Material Path in disk.
		*/
		Material(const std::string& materialPath);

		/**
		* @brief Destructor Function.
		*/
		virtual ~Material();

		/**
		* @brief Serialize this class data to a disk file(.material).
		* @todo Finish it.
		*/
		void Serialize();

		/**
		* @brief Deserialize the data from a disk file(.material) to this class.
		* @todo Finish it.
		*/
		void Deserialize();

		const std::string& GetName() { return m_MaterialPath; }

		/**
		* @brief Get material material descriptor set.
		* @return Returns the material descriptorSet.
		* @note Must call BuildMaterial() first.
		*/
		std::unordered_map<uint32_t, std::shared_ptr<VulkanDescriptorSet>>& GetMaterialDescriptorSet();

		/**
		* @brief Get material shader path.
		* @param[in] stage Shader stage name.
		* @return Returns the stage shader path that needed.
		*/
		const std::vector<std::string>& GetShaderPath(const std::string& stage);

		/**
		* @brief Get material shader path.
		* @return Returns all the stage shader path that needed.
		*/
		const std::unordered_map<std::string, std::vector<std::string>>& GetShaderPath() { return m_Shaders; }

		/**
		* @brief Get material texture parameters.
		* @return Returns the material texture parameters.
		*/
		std::unordered_map<std::string, TextureParam>& GetTextureParams() { return m_TextureParams; }

		/**
		* @brief Get material constant parameters.
		* @return Returns the material constant parameters.
		*/
		scl::linked_unordered_map<std::string, ConstantParam>& GetConstantParams() { return m_ConstantParams; }

		uint64_t GetConstantParamsAddress();
		
		/**
		* @brief This interface need to be overwritten by specific material.
		* It defines how we build texture and descriptor set.
		* @todo empty texture.
		*/
		void BuildMaterial();

		/**
		* @brief Allow MaterialLoader access all data.
		*/
		friend class MaterialLoader;

		/**
		* @brief Allow MeshPack access all data.
		*/
		friend class MeshPack;

	protected:

		/**
		* @beief Material path
		*/
		std::string m_MaterialPath;

		/**
		* @brief Shader path
		* 0 - vertex shader
		* 1 - fragment shader
		* ... other shader
		* Key: shader usage, Value: shader file name.
		*/
		std::unordered_map<std::string, std::vector<std::string>> m_Shaders;

		/**
		* @brief Texture parameters.
		* Key: parameter name, Value: parameter value.
		*/
		std::unordered_map<std::string, TextureParam> m_TextureParams;

		/**
		* @brief Constant parameters.
		* Key: parameter name, Value: parameter value.
		*/
		scl::linked_unordered_map<std::string, ConstantParam> m_ConstantParams;

		/**
		* @brief m_ConstantParams's VkBuffer.
		* Key: set, Value: VkBuffer.
		*/
		std::unordered_map<UInt2, std::unique_ptr<VulkanBuffer>> m_Buffers;

		/**
		* @brief m_Buffers's c++ data container.
		* Key: set, Value: scl::runtime_memory_block.
		*/
		std::unordered_map<UInt2, scl::runtime_memory_block> m_Buffermemoryblocks;
	};
}