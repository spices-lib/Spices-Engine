/**
* @file Material.cpp.
* @brief The Material Class Implementation.
* @author Spiecs.
*/

#include "Pchheader.h"
#include "Material.h"
#include "Render/Vulkan/VulkanRenderBackend.h"
#include "Core/Reflect/TypeReflect.h"

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
		
		m_ConstantParams.for_each([&](const std::string& k, const ConstantParam& v) {
			layouts[v.set][v.binding].size += StrType2Size(v.paramType);
			layouts[v.set][v.binding].count = 1;
			layouts[v.set][v.binding].type = DescriptorSetBindingInfoHelp::Type::Buffer;
		});

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

		m_ConstantParams.for_each([&](const std::string& k, const ConstantParam& v) {
			Int2 int2(v.set, v.binding);
			
			m_Buffers[int2] = nullptr;
			m_Buffermemoryblocks[int2].AddElement(k, v.paramType);
		});
		
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

		for(auto& pair : m_Buffermemoryblocks)
		{
			pair.second.Build();
			pair.second.for_each([&](const std::string& name, void* pt) {
				ConstantParam& ref = m_ConstantParams.find_value(name);

				size_t size;
				switch(ref.paramType)
				{
				case "float4" :
					*reinterpret_cast<glm::vec4*>(pt) = std::any_cast<glm::vec4>(ref.paramValue);
					break;
				case "float3" :
					*reinterpret_cast<glm::vec4*>(pt) = glm::vec4(std::any_cast<glm::vec3>(ref.paramValue), 0.0f);
					break;
				case "float2" :
					*reinterpret_cast<glm::vec2*>(pt) = std::any_cast<glm::vec2>(ref.paramValue);
					break;
				case "float" :
					*reinterpret_cast<float*>(pt) = std::any_cast<float>(ref.paramValue);
					break;
				}
			});

			m_Buffers[pair.first]->WriteToBuffer(pair.second.GetAddr());
			m_Buffers[pair.first]->Flush();
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