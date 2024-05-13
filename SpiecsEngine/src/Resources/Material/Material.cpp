/**
* @file Material.cpp.
* @brief The Material Class Implementation.
* @author Spiecs.
*/

#include "Pchheader.h"
#include "Material.h"
#include "Render/Vulkan/VulkanRenderBackend.h"

namespace Spiecs {

	Material::Material(const std::string& materialPath)
		: m_MaterialPath(materialPath)
	{
		Dserialize();
	}

	Material::~Material()
	{
		vkFreeDescriptorSets(VulkanRenderBackend::GetState().m_Device, VulkanRenderBackend::GetDescriptorPool()->GetPool(), m_DescriptorSets.size(), m_DescriptorSets.data());
	}

	void Material::Serialize()
	{
		if (m_MaterialPath.empty())
		{
			SPIECS_CORE_INFO("Please do not do that!");
		}
	}

	void Material::Dserialize()
	{
		if (m_MaterialPath.empty())
		{
			SPIECS_CORE_WARN("Material::m_MaterialPath is empty.");
			return;
		}

		MaterialLoader::Load(m_MaterialPath, this);
	}

	void Material::BuildMaterial()
	{
		/**
		* @brief Data Like this:
		* [count0,   count1],  [count0,   count1]
		* [binding0, binding1],[binging0, binging1]     
		*		 [set0    ,        set1]
		*/
		std::vector<std::vector<DescriptorSetBindingInfoHelp>> layouts;
		for(auto& pair : m_TextureParams)
		{
			TextureParam& tp = pair.second;

			if(layouts.size() < tp.set + 1)              layouts.resize(tp.set + 1);
			if(layouts[tp.set].size() < tp.binding + 1)  layouts[tp.set].resize(tp.binding + 1);

			layouts[tp.set][tp.binding].count = glm::max(layouts[tp.set][tp.binding].count, tp.index + 1);
			layouts[tp.set][tp.binding].size = 0;
			layouts[tp.set][tp.binding].type = DescriptorSetBindingInfoHelp::Type::Image;
		}
		for(auto& pair : m_ConstantParams)
		{
			ConstantParam& cp = pair.second;

			if(layouts.size() < cp.set + 1)              layouts.resize(cp.set + 1);
			if(layouts[cp.set].size() < cp.binding + 1)  layouts[cp.set].resize(cp.binding + 1);

			if     (cp.paramType == "float4") layouts[cp.set][cp.binding].size += sizeof(glm::vec4);
			else if(cp.paramType == "float3") layouts[cp.set][cp.binding].size += sizeof(glm::vec3);
			else if(cp.paramType == "float2") layouts[cp.set][cp.binding].size += sizeof(glm::vec2);
			else if(cp.paramType == "float")  layouts[cp.set][cp.binding].size += sizeof(float);
			else
			{
				SPIECS_CORE_WARN("Material::BuildMaterial(): Unknown paramType.");
			}

			layouts[cp.set][cp.binding].count = 1;
			layouts[cp.set][cp.binding].type = DescriptorSetBindingInfoHelp::Type::Buffer;
		}

		std::vector<std::vector<std::vector<VkDescriptorImageInfo>>> imageInfos;
		for(auto& pair : m_TextureParams)
		{
			TextureParam& tp = pair.second;

			if(tp.textureType == "Texture2D") tp.texture = ResourcePool<Texture>::Load<Texture2D>(tp.texturePath);
			else
			{
				SPIECS_CORE_ERROR("Material::BuildMaterial(): Invalid textureType.");
			}

			if(imageInfos.size() < tp.set + 1)                        imageInfos.resize(tp.set + 1);
			if(imageInfos[tp.set].size() < tp.binding + 1)            imageInfos[tp.set].resize(tp.binding + 1);
			if(imageInfos[tp.set][tp.binding].size() < tp.index + 1)  imageInfos[tp.set][tp.binding].resize(tp.index + 1);
			
			imageInfos[tp.set][tp.binding][tp.index] = *tp.texture->GetResource<VulkanImage>()->GetImageInfo();
		}
		
		for(auto& pair : m_ConstantParams)
		{
			ConstantParam& cp = pair.second;

			m_Buffers[{cp.set, cp.binding} ] = nullptr;
		}
		
		std::vector<std::vector<VkDescriptorBufferInfo>> bufferInfos;
		for(auto& pair : m_Buffers)
		{
			pair.second = std::make_unique<VulkanBuffer>(
				VulkanRenderBackend::GetState(),
				layouts[pair.first.x][pair.first.y].size,
				VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
				VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT
			);

			if(bufferInfos.size() < pair.first.x + 1)                     bufferInfos.resize(pair.first.x + 1);
			if(bufferInfos[pair.first.x].size() < pair.first.y + 1)  bufferInfos[pair.first.x].resize(pair.first.y + 1);
			
			bufferInfos[pair.first.x][pair.first.y] = *pair.second->GetBufferInfo();
		}
		
		for(int i = 0; i < layouts.size(); i++)
		{
			if(layouts[i].empty()) continue;
			
			std::vector<VkDescriptorSetLayoutBinding> descriptorSetLayoutBindings;
			VkDescriptorSetLayout m_DescriptorSetLayout;
			
			for(int j = 0; j < layouts[i].size(); j++)
			{
				VkDescriptorSetLayoutBinding layoutBinding{};
				layoutBinding.binding = j;

				switch( layouts[i][j].type)
				{
				case DescriptorSetBindingInfoHelp::Type::Image:
					layoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
					break;
				case DescriptorSetBindingInfoHelp::Type::Buffer:
					layoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
					break;
				}

				layoutBinding.descriptorCount = layouts[i][j].count;
				layoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

				descriptorSetLayoutBindings.push_back(layoutBinding);
			}

			VkDescriptorSetLayoutCreateInfo descriptorSetLayoutCreateInfo{};
			descriptorSetLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
			descriptorSetLayoutCreateInfo.bindingCount = static_cast<uint32_t>(descriptorSetLayoutBindings.size());
			descriptorSetLayoutCreateInfo.pBindings = descriptorSetLayoutBindings.data();

			VK_CHECK(vkCreateDescriptorSetLayout(VulkanRenderBackend::GetState().m_Device, &descriptorSetLayoutCreateInfo, nullptr, &m_DescriptorSetLayout));

			VkDescriptorSetAllocateInfo allocInfo{};
			allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
			allocInfo.descriptorPool = VulkanRenderBackend::GetDescriptorPool()->GetPool();
			allocInfo.pSetLayouts = &m_DescriptorSetLayout;
			allocInfo.descriptorSetCount = 1;

			m_DescriptorSets.resize(m_DescriptorSets.size() + 1);
			
			VK_CHECK(vkAllocateDescriptorSets(VulkanRenderBackend::GetState().m_Device, &allocInfo, &m_DescriptorSets[m_DescriptorSets.size() - 1]));
			
			for(int j = 0; j < layouts[i].size(); j++)
			{
				VkWriteDescriptorSet write{};
				write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
				write.dstBinding = j;
				write.dstSet = m_DescriptorSets[m_DescriptorSets.size() - 1];

				switch(layouts[i][j].type)
				{
				case DescriptorSetBindingInfoHelp::Type::Image:
					write.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
					write.pImageInfo = imageInfos[i][j].data();
					write.descriptorCount = static_cast<uint32_t>(imageInfos[i][j].size());
					break;
				case DescriptorSetBindingInfoHelp::Type::Buffer:
					write.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
					write.pBufferInfo = &bufferInfos[i][j];
					write.descriptorCount = 1;
					break;
				}
				
				vkUpdateDescriptorSets(VulkanRenderBackend::GetState().m_Device, 1, &write, 0, nullptr);
			}

			vkDestroyDescriptorSetLayout(VulkanRenderBackend::GetState().m_Device, m_DescriptorSetLayout, nullptr);
		}
	}
}