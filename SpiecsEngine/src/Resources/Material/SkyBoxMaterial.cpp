#include "Pchheader.h"
#include "SkyBoxMaterial.h"
#include "Render/Vulkan/VulkanRenderBackend.h"
#include "Resources/Texture/Texture2DCube.h"

namespace Spiecs {

	void SkyBoxMaterial::BuildMaterial()
	{
		if (m_Shaders["vertShader"].empty()) __debugbreak();
		if (m_Shaders["fragShader"].empty()) __debugbreak();

		VulkanDescriptorSetLayout::Builder setLayoutBuilder = VulkanDescriptorSetLayout::Builder()
			.AddBinding(0, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT, 1);
		std::array<VkDescriptorImageInfo, 1> imageInfos{};

		for (auto& pair : m_TextureParams)
		{
			TextureParam& tp = pair.second;

			if (tp.isInUse.has_value() && tp.isInUse.value())
			{
				if (tp.texturePath.empty()) __debugbreak();

				tp.texture = std::make_shared<Texture2DCube>(tp.texturePath);
				imageInfos[tp.index] = *tp.texture->GetResource<VulkanImage>()->GetImageInfo();
			}
			else
			{
				// TODO: Load a Default Texture
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
		write.descriptorCount = imageInfos.size();

		vkUpdateDescriptorSets(VulkanRenderBackend::GetState().m_Device, 1, &write, 0, nullptr);
	}

}