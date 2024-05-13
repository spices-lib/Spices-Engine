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
		std::unordered_map<uint32_t, std::unordered_map<uint32_t, DescriptorSetBindingInfoHelp>> layouts;
		for(auto& pair : m_TextureParams)
		{
			TextureParam& tp = pair.second;
			
			layouts[tp.set][tp.binding].count = glm::max(layouts[tp.set][tp.binding].count, tp.index + 1);
			layouts[tp.set][tp.binding].size = 0;
			layouts[tp.set][tp.binding].type = DescriptorSetBindingInfoHelp::Type::Image;
		}
		for(auto& pair : m_ConstantParams)
		{
			ConstantParam& cp = pair.second;

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

		std::unordered_map<uint32_t, std::unordered_map<uint32_t, std::vector<VkDescriptorImageInfo>>> imageInfos;
		for(auto& pair : m_TextureParams)
		{
			TextureParam& tp = pair.second;

			if(tp.textureType == "Texture2D")
			{
				std::shared_ptr<Texture> texture = ResourcePool<Texture>::Load<Texture2D>(tp.texturePath);

				if(imageInfos[tp.set][tp.binding].size() < tp.index + 1)  imageInfos[tp.set][tp.binding].resize(tp.index + 1);
			
				imageInfos[tp.set][tp.binding][tp.index] = *texture->GetResource<VulkanImage>()->GetImageInfo();
			}
			else
			{
				SPIECS_CORE_ERROR("Material::BuildMaterial(): Invalid textureType.");
			}
		}
		
		for(auto& pair : m_ConstantParams)
		{
			ConstantParam& cp = pair.second;

			m_Buffers[{cp.set, cp.binding} ] = nullptr;
		}
		
		std::unordered_map<uint32_t, std::unordered_map<uint32_t, VkDescriptorBufferInfo>> bufferInfos;
		for(auto& pair : m_Buffers)
		{
			pair.second = std::make_unique<VulkanBuffer>(
				VulkanRenderBackend::GetState(),
				layouts[pair.first.x][pair.first.y].size,
				VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
				VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT
			);
			pair.second->Map();
			
			bufferInfos[pair.first.x][pair.first.y] = *pair.second->GetBufferInfo();
		}
		
		for(auto& layout : layouts)
		{
			m_DescriptorSets[layout.first] = std::make_shared<VulkanDescriptorSet>(VulkanRenderBackend::GetState(), VulkanRenderBackend::GetDescriptorPool());
			
			for(auto& pair : layout.second)
			{
				switch(pair.second.type)
				{
				case DescriptorSetBindingInfoHelp::Type::Image:
					m_DescriptorSets[layout.first]->AddBinding(pair.first, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT, pair.second.count);
					break;
				case DescriptorSetBindingInfoHelp::Type::Buffer:
					m_DescriptorSets[layout.first]->AddBinding(pair.first, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_FRAGMENT_BIT, pair.second.count);
					break;
				}
			}
			
			m_DescriptorSets[layout.first]->BuildDescriptorSet();
			m_DescriptorSets[layout.first]->UpdateDescriptorSet(imageInfos[layout.first], bufferInfos[layout.first]);
		}
	}
}