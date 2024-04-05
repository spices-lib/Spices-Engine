#include "pchheader.h"
#include "Material.h"
#include "Render/Vulkan/VulkanRenderBackend.h"

namespace Spiecs {

	void Material::LoadMaterial(const std::string& materialPath)
	{
		m_MaterialPath = materialPath;

		MaterialLoader::Load(materialPath, this);
		BuildMaterial();
	}

	void Material::Reload()
	{
		if (m_MaterialPath.empty())
		{
			SPIECS_LOG("Please do not do that!");
		}

		MaterialLoader::Load(m_MaterialPath, this);
		BuildMaterial();
	}

	void Material::BuildMaterial()
	{
		if (m_VertShaderPath.empty()) __debugbreak();
		if (m_FragShaderPath.empty()) __debugbreak();

		m_VertShaderModule = std::make_unique<VulkanShaderModule>(VulkanRenderBackend::GetState(), m_VertShaderPath);
		m_FragShaderModule = std::make_unique<VulkanShaderModule>(VulkanRenderBackend::GetState(), m_FragShaderPath);

		for (int i = 0; i < m_TexturePaths.size(); i++)
		{
			if (m_TextureIsUse[i].value())
			{
				if (m_TexturePaths[i].empty()) __debugbreak();
				m_Textures[i] = std::make_unique<Texture2D>(m_TexturePaths[i]);
			}
		}
	}
}