/**
* @file Material.h.
* @brief The Material Class Definitions.
* @author Spiecs.
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

namespace Spiecs {

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
		* @breif Which arrayindex this texture will use.
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
	* This class contains a banch of parameter and shader, alse descriptor.
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
			
			DescriptorSetBindingInfoHelp() {};
			
			uint32_t count = 0;
			size_t size = 0;
			Type type = Type::Buffer;
		};
		
	public:

		/**
		* @brief Constructor Function.
		*/
		Material() {};

		/**
		* @brief Constructor Function.
		* Dserialize immediatelly.
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
		* @brief Dserialize the data from a disk file(.material) to this class.
		* @todo Finish it.
		*/
		void Dserialize();

		inline const std::string& GetName() { return m_MaterialPath; };

		/**
		* @brief Get material material descriptorset.
		* @return Returns the material descriptorSet.
		* @note Must call BuildMaterial() first.
		*/
		std::unordered_map<uint32_t, std::shared_ptr<VulkanDescriptorSet>>& GetMaterialDescriptorSet();

		/**
		* @brief Get material shader path.
		* @param[in] stage Shader stage name.
		* @return Returns the stage shader path that needed.
		*/
		const std::string& GetShaderPath(const std::string& stage);

		/**
		* @brief Get material shader path.
		* @return Returns all the stage shader path that needed.
		*/
		inline const std::unordered_map<std::string, std::string>& GetShaderPath() { return m_Shaders; };

		/**
		* @brief Get material texture parameters.
		* @return Returns the material texture parameters.
		*/
		inline std::unordered_map<std::string, TextureParam>& GetTextureParams() { return m_TextureParams; };

		/**
		* @brief Get material constant parameters.
		* @return Returns the material constant parameters.
		*/
		inline scl::linked_unordered_map<std::string, ConstantParam>& GetConstantParams() { return m_ConstantParams; };

		/**
		* @brief This interface need to be overwitten by specific material.
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
		* 0 - vertshader
		* 1 - fragshader
		* ... othershader
		* Key: shaderusage, Value: shaderfilename.
		*/
		std::unordered_map<std::string, std::string> m_Shaders;

		/**
		* @brief Texture parameters.
		* Key: paramname, Value: paramvalue.
		*/
		std::unordered_map<std::string, TextureParam> m_TextureParams;

		/**
		* @brief Constant parameters.
		* Key: paramname, Value: paramvalue.
		*/
		scl::linked_unordered_map<std::string, ConstantParam> m_ConstantParams;

		/**
		* @brief m_ConstantParams's VkBuffer.
		* Key: set, Value: VkBuffer.
		*/
		std::unordered_map<Int2, std::unique_ptr<VulkanBuffer>> m_Buffers;

		/**
		* @brief m_Buffers's c++ data container.
		* Key: set, Value: scl::runtime_memory_block.
		*/
		std::unordered_map<Int2, scl::runtime_memory_block> m_Buffermemoryblocks;
	};
}