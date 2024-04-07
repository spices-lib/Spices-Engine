#pragma once
#include "Core/Core.h"
#include "Texture/Texture2D.h"
#include "Render/Resources/Loader/MaterialLoader.h"
#include "Render/Vulkan/VulkanShaderModule.h"
#include "Render/Resources/Texture/Texture2D.h"
#include "glm/glm.hpp"

#include <optional>

namespace Spiecs {

	class Material
	{
	public:
		Material() {};
		virtual ~Material() {};

		void LoadMaterial(const std::string& materialPath);
		void Reload();
		VkDescriptorSet& GetTextureDescriptorSet(uint32_t set, uint32_t binding);

		const std::array<std::shared_ptr<Texture2D>, 3>& GetTextures() { return m_Textures; };

	private:
		void BuildMaterial();

		friend class MaterialLoader;
	private:
		std::string m_MaterialPath;

		// Shader
		std::string m_VertShaderPath;
		std::string m_FragShaderPath;

		// Texture
		std::array<std::optional<bool>, 3> m_TextureIsUse;
		std::array<std::string, 3> m_TexturePaths;
		std::array<glm::vec3, 3> m_TextureV3{};
		std::array<uint32_t[2], 3> m_TextureSetBinding{};

		// Parameter
		std::array<float, 3> m_TextureIntensity;

		// BuildData
		std::array<std::shared_ptr<Texture2D>, 3> m_Textures;
	};
}