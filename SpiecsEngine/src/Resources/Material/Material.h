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

#include <glm/glm.hpp>
#include <unordered_map>

namespace Spiecs {

	class VulkanBuffer;
	
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
			
			uint32_t count;
			uint32_t size;
			Type type;
		};
		
		/**
		* @brief This struct's data is defined from .material file.
		*/
		struct TextureParam
		{
			/********************Can not be Serialize*******************/
			
			/**
			* @brief Texture shared pointer.
			*/
			std::shared_ptr<Texture> texture;

			/***********************************************************/
			
			/********************Can be Serialize***********************/
			
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

			/***********************************************************/
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

		/**
		* @brief Get material material descriptorset.
		* @return Returns the material descriptorSet.
		* @note Must call BuildMaterial() first.
		*/
		inline std::vector<VkDescriptorSet>& GetMaterialDescriptorSet() { return m_DescriptorSets; };

		/**
		* @brief Get material shader path.
		* @return Returns all stage shader path that needed.
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
		inline std::unordered_map<std::string, ConstantParam>& GetConstantParams() { return m_ConstantParams; };

	private:

		/**
		* @brief This interface need to be overwitten by specific material.
		* It defines how we build texture and descriptor set.
		* @todo empty texture.
		*/
		virtual void BuildMaterial();

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
		* @brief Descriptorset, usually is set 1 binding 0
		* only contain texture
		* only one
		* @todo multiple descriptor set.
		*/
		std::vector<VkDescriptorSet> m_DescriptorSets{};

		/**
		* @brief Shader path
		* 0 - vertshader
		* 1 - fragshader
		* ... othershader
		*/
		std::unordered_map<std::string, std::string> m_Shaders;

		/**
		* @brief Texture parameters.
		*/
		std::unordered_map<std::string, TextureParam> m_TextureParams;

		/**
		* @brief Constant parameters.
		*/
		std::unordered_map<std::string, ConstantParam> m_ConstantParams;

		std::unordered_map<Int2, std::shared_ptr<VulkanBuffer>> m_Buffers;
	};
}