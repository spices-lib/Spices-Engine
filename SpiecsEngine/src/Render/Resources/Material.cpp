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

		VulkanDescriptorSetLayout::Builder setLayoutBuilder = VulkanDescriptorSetLayout::Builder()
			.AddBinding(0, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT, 3);
		std::array<VkDescriptorImageInfo, 3> imageInfos{};

		for (int i = 0; i < 3; i++)
		{
			if (m_TextureIsUse[i].has_value() && m_TextureIsUse[i].value())
			{
				if (m_TexturePaths[i].empty()) __debugbreak();
				m_Textures[i] = std::make_shared<Texture2D>(m_TexturePaths[i]);
				int index = m_TextureSetBinding[i][2];
				imageInfos[index] = *m_Textures[i]->GetResources()->GetImageInfo();
			}
		}

		std::unique_ptr<VulkanDescriptorSetLayout> setLayout = setLayoutBuilder.Build(VulkanRenderBackend::GetState());

		VulkanRenderBackend::GetDescriptorPool()->allocateDescriptor(setLayout->GetDescriptorSetLayout(), m_DescriptorSet);

		VkWriteDescriptorSet write{};
		write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		write.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		write.dstBinding = 0;
		write.dstSet = m_DescriptorSet;
		write.pImageInfo = imageInfos.data();
		write.descriptorCount = 3;

		vkUpdateDescriptorSets(VulkanRenderBackend::GetState().m_Device, 1, &write, 0, nullptr);
	}
}