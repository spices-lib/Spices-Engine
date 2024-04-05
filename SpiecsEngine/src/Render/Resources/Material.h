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

		// Parameter
		std::array<float, 3> m_TextureIntensity;

		// BuildData
		std::unique_ptr<VulkanShaderModule> m_VertShaderModule;
		std::unique_ptr<VulkanShaderModule> m_FragShaderModule;

		std::array<std::unique_ptr<Texture2D>, 3> m_Textures;
	};
}