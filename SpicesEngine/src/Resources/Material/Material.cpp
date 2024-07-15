/**
* @file Material.cpp.
* @brief The Material Class Implementation.
* @author Spices.
*/

#include "Pchheader.h"
#include "Material.h"
#include "Render/Vulkan/VulkanRenderBackend.h"
#include "Core/Reflect/TypeReflect.h"
#include "Render/Renderer/DescriptorSetManager/DescriptorSetManager.h"
#include "Render/Renderer/RendererManager.h"
#include "Core/Library/StringLibrary.h"
#include "Render/Renderer/Renderer.h"


namespace Spices {

	Material::Material(const std::string& materialPath)
		: m_MaterialPath(materialPath)
	{
		/**
		* @breif Call Dserialize() while in Constructor.
		*/
		Deserialize();
	}

	Material::~Material()
	{
		DescriptorSetManager::UnLoad(m_MaterialPath);
	}

	void Material::Serialize()
	{
		if (m_MaterialPath.empty())
		{
			SPICES_CORE_INFO("Please do not do that!");
		}
	}

	void Material::Deserialize()
	{
		if (m_MaterialPath.empty())
		{
			SPICES_CORE_WARN("Material::m_MaterialPath is empty.");
			return;
		}

		MaterialLoader::Load(m_MaterialPath, this);
	}

	std::unordered_map<uint32_t, std::shared_ptr<VulkanDescriptorSet>>& Material::GetMaterialDescriptorSet()
	{
		return DescriptorSetManager::GetByName(m_MaterialPath);
	}

	const std::vector<std::string>& Material::GetShaderPath(const std::string& stage)
	{
		if (m_Shaders.find(stage) == m_Shaders.end())
		{
			std::stringstream ss;
			ss << "Material: " << m_MaterialPath << " : Not find sush shader: " << stage;

			SPICES_CORE_WARN(ss.str());
		}

		return m_Shaders[stage];
	}

	uint64_t Material::GetMaterialParamsAddress() const
	{
		if(m_MaterialParameterBuffer == nullptr) return 0;
		else return m_MaterialParameterBuffer->GetAddress();
	}

	void Material::BuildMaterial()
	{
		/**
		* @brief If ReBuild, need clear old descripotrset first.
		*/
		DescriptorSetManager::UnLoad(m_MaterialPath);
		m_MaterialParameterBuffer = nullptr;

		/**
		* @brief Container like that: Set - [ binding - [index - (type, count, size)]].
		*/
		std::unordered_map<uint32_t, std::unordered_map<uint32_t, DescriptorSetBindingInfoHelp>> layouts;

		/**
		* @brief Fill in textureParams to layouts.
		* Feel free to iter the container without order.
		*/
		for (auto& pair : m_TextureParams)
		{
			TextureParam& tp = pair.second;

			layouts[tp.set][tp.binding].count = glm::max(layouts[tp.set][tp.binding].count, tp.index + 1);  // set to max num.
			layouts[tp.set][tp.binding].size = 0;                                                              // though it is a image param, set to 0
			layouts[tp.set][tp.binding].type = DescriptorSetBindingInfoHelp::Type::Image;                      // set to image type.
		}

		/**
		* @brief Fill in constantParams to layouts.
		* Need iter the container in order for correct memory alignment.
		*/
		m_ConstantParams.for_each([&](const std::string& k, const ConstantParam& v) {
			layouts[v.set][v.binding].count = 1;                                                               // set to 1, buffer type always 1.
			layouts[v.set][v.binding].size += StrType2Size(v.paramType);                                       // set to add the size of value type.
			layouts[v.set][v.binding].type = DescriptorSetBindingInfoHelp::Type::Buffer;                      // set to buffer type.
			return false;
		});
		
		/**
		* @brief Container like that: Set - [ binding - [imageInfo0, imageInfo1, imageInfo2...]].
		*/
		std::unordered_map<uint32_t, std::unordered_map<uint32_t, std::vector<VkDescriptorImageInfo>>> imageInfos;

		/**
		* @brief Fill in textureParams to imageInfos.
		*/
		for (auto& pair : m_TextureParams)
		{
			TextureParam& tp = pair.second;

			/**
			* @brief Only work with Texture2D now.
			* @todo more type support, reflection.
			*/
			if (tp.textureType == "Texture2D")
			{
				/**
				* @brief Apply for a tecture from Texture's ResourcePool.
				*/
				std::shared_ptr<Texture> texture = ResourcePool<Texture>::Load<Texture2D>(tp.texturePath);

				/**
				* @brief Expand the vector to matched size.
				*/
				if (imageInfos[tp.set][tp.binding].size() < tp.index + 1)  imageInfos[tp.set][tp.binding].resize(tp.index + 1);

				/**
				* @brief Fill in imageinfo.
				*/
				imageInfos[tp.set][tp.binding][tp.index] = *texture->GetResource<VulkanImage>()->GetImageInfo();
			}

			/**
			* @brief Not supported format.
			*/
			else
			{
				SPICES_CORE_ERROR("Material::BuildMaterial(): Invalid textureType.");
			}
		}

		/**
		* @brief Iter the constantParams for creating a host buffer and it's data memoryblock.
		*/
		m_ConstantParams.for_each([&](const std::string& k, const ConstantParam& v) {
			UInt2 int2(v.set, v.binding);

			/**
			* @brief Create the key to map.
			*/
			m_Buffers[int2] = nullptr;

			/**
			* @brief Create the key to map, and add element to value.
			*/
			if (!m_Buffermemoryblocks.has_key(int2))
			{
				m_Buffermemoryblocks.push_back(int2, scl::runtime_memory_block());
			}
			m_Buffermemoryblocks.find_value(int2)->add_element(k, v.paramType);

			return false;
		});

		/**
		* @brief Container like that: Set - [ binding - VkDescriptorBufferInfo].
		*/
		std::unordered_map<uint32_t, std::unordered_map<uint32_t, VkDescriptorBufferInfo>> bufferInfos;

		/**
		* @brief Iter m_Buffers for creating VulkanBuffer and fill in VkDescriptorBufferInfo to bufferInfos.
		*/
		for (auto& pair : m_Buffers)
		{
			/**
			* @brief Creating VulkanBuffer.
			*/
			pair.second = std::make_unique<VulkanBuffer>(
				VulkanRenderBackend::GetState(),
				layouts[pair.first.x][pair.first.y].size,
				VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT |
				VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT,
				VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT
			);

			/**
			* @brief Map with host memory and video memory.
			*/
			pair.second->Map();

			/**
			* @brief fill in bufferInfos.
			*/
			bufferInfos[pair.first.x][pair.first.y] = *pair.second->GetBufferInfo();
		}

		/**
		* @brief Create a buffer to store all address and index.
		*/
		uint64_t size = m_Buffers.size() * sizeof(uint64_t); // + m_TextureParams.size() * sizeof(uint);
		if(size != 0)
		{
			m_MaterialParameterBuffer = std::make_unique<VulkanBuffer>(
				VulkanRenderBackend::GetState(),
				size,
				VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT |
				VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT,
				VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT
			);
			m_MaterialParameterBuffer->Map();
		}
		
		/**
		* @brief Iter m_Buffermemoryblocks for filling data on m_ConstantParams.
		*/
		int index = 0;
		m_Buffermemoryblocks.for_each([&](const UInt2& k, scl::runtime_memory_block& v){
			
			/**
			* @brief Malloc the memory to this container.
			*/
			v.build();

			/**
			* @brief Iter the block's element and filling data.
			*/
			v.for_each([&](const std::string& name, void* pt) {

				/**
				* @brief Get constantParam by param's name.
				*/
				ConstantParam& ref = *m_ConstantParams.find_value(name);

				/**
				* @brief Fill in data to memory block.
				*/
				if (ref.paramType == "float4")
				{
					*static_cast<glm::vec4*>(pt) = std::any_cast<glm::vec4>(ref.paramValue);
				}
				else if (ref.paramType == "float3")
				{
					*static_cast<glm::vec3*>(pt) = std::any_cast<glm::vec3>(ref.paramValue);
				}
				else if (ref.paramType == "float2")
				{
					*static_cast<glm::vec2*>(pt) = std::any_cast<glm::vec2>(ref.paramValue);
				}
				else if (ref.paramType == "float")
				{
					*static_cast<float*>(pt) = std::any_cast<float>(ref.paramValue);
				}
				return false;
			});

			/**
			* @brief Write the memeoryblock's data to m_Buffers.
			*/
			m_Buffers[k]->WriteToBuffer(v.get_addr());

			/**
			* @brief Flush the m_Buffers's memory.
			*/
			m_Buffers[k]->Flush();
			
			m_MaterialParameterBuffer->WriteToBuffer(&m_Buffers[k]->GetAddress(), sizeof(uint64_t), index * sizeof(uint64_t));
			m_MaterialParameterBuffer->Flush();

			index++;

			return false;
		});

		/**
		* @brief Iter layouts set for creating descriptorset.
		*/
		for (auto& layout : layouts)
		{
			/**
			* @brief Creating a empty VulkanDescriptorSet.
			*/
			auto descriptorSet = DescriptorSetManager::Registy(m_MaterialPath, layout.first);

			/**
			* @brief Iter layouts binding for Add Binding.
			*/
			for (auto& pair : layout.second)
			{
				switch (pair.second.type)
				{
				case DescriptorSetBindingInfoHelp::Type::Image:
					descriptorSet->AddBinding(pair.first, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT, pair.second.count);
					break;
				case DescriptorSetBindingInfoHelp::Type::Buffer:
					descriptorSet->AddBinding(pair.first, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_FRAGMENT_BIT, pair.second.count);
					break;
				}
			}

			/**
			* @brief AllocateDescriptorSet for Pool.
			*/
			descriptorSet->BuildDescriptorSet(m_MaterialPath);

			/**
			* @brief UpdateDescriptorSet.
			*/
			descriptorSet->UpdateDescriptorSet(imageInfos[layout.first], bufferInfos[layout.first]);
		}

		/**
		* @brief Create PipelineLayout.
		*/
		std::vector<std::string> sv = StringLibrary::SplitString(m_MaterialPath, '.');
		auto renderer = RendererManager::GetRenderer(sv[0]);
		renderer->RegistryMaterial(m_MaterialPath, sv[1]);
	}
}