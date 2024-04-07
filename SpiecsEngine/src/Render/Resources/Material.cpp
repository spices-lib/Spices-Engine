#include "pchheader.h"
#include "Material.h"

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

	VkDescriptorSet& Material::GetTextureDescriptorSet(uint32_t set, uint32_t binding)
	{
		for (int i = 0; i < 3; i++)
		{
			if (m_TextureSetBinding[i][0] == set && m_TextureSetBinding[i][1] == binding)
			{
				return m_Textures[i]->GetResources()->GetDescriptorSet();
			}
		}
	}

	void Material::BuildMaterial()
	{
		if (m_VertShaderPath.empty()) __debugbreak();
		if (m_FragShaderPath.empty()) __debugbreak();

		for (int i = 0; i < m_TexturePaths.size(); i++)
		{
			if (m_TextureIsUse[i].value())
			{
				if (m_TexturePaths[i].empty()) __debugbreak();
				m_Textures[i] = std::make_shared<Texture2D>(m_TexturePaths[i]);
				m_Textures[i]->GetResources()->CreateDescriptorSet(m_TextureSetBinding[i][0], m_TextureSetBinding[i][1]);
			}
		}
	}
}