#pragma once
#include "Core/Core.h"
#include "Render/Resources/Texture/Texture2D.h"
#include "Render/Resources/Loader/MaterialLoader.h"
#include "Render/Vulkan/VulkanShaderModule.h"
#include "Render/Resources/Texture/Texture.h"
#include "glm/glm.hpp"

#include <optional>
#include <unordered_map>

namespace Spiecs {

	class Material
	{
	public:
		struct TextureParam
		{
			std::optional<uint32_t> isInUse;
			std::shared_ptr<Texture> texture;
			std::string texturePath;
			uint32_t set;
			uint32_t binding;
			uint32_t index;
			glm::vec3 constant;
			float intensity;
		};

	public:
		Material() {};
		virtual ~Material() {};

		void LoadMaterial(const std::string& materialPath);
		void Reload();

		VkDescriptorSet& GetMaterialDescriptorSet() { return m_DescriptorSet; };
		std::unordered_map<std::string, std::string>& GetShaderPath() { return m_Shaders; };
		std::unordered_map<std::string, TextureParam>& GetTextureParams() { return m_TextureParams; };

	private:
		virtual void BuildMaterial() {};

		friend class MaterialLoader;

	protected:

		/**
		* @beief material path
		*/
		std::string m_MaterialPath;

		/**
		* @brief descriptorset, usually is set 1 binding 0
		* only contain texture
		* only one
		*/
		VkDescriptorSet m_DescriptorSet;

		/**
		* @brief shader path
		* 0 - vertshader
		* 1 - fragshader
		* ... othershader
		*/
		std::unordered_map<std::string, std::string> m_Shaders;

		std::unordered_map<std::string, TextureParam> m_TextureParams;
	};
}