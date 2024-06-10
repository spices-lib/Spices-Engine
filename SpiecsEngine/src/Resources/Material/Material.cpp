/**
* @file Material.cpp.
* @brief The Material Class Implementation.
* @author Spiecs.
*/

#include "Pchheader.h"
#include "Material.h"
#include "Render/Vulkan/VulkanRenderBackend.h"
#include "Core/Reflect/TypeReflect.h"
#include "Render/Renderer/DescriptorSetManager/DescriptorSetManager.h"
#include "Render/Renderer/RendererManager.h"
#include "Core/Library/StringLibrary.h"
#include "Render/Renderer/Renderer.h"


namespace Spiecs {

	Material::Material(const std::string& materialPath)
		: m_MaterialPath(materialPath)
	{
		/**
		* @breif Call Dserialize() while in Constructor.
		*/
		Dserialize();
	}

	Material::~Material()
	{
		DescriptorSetManager::UnLoad(m_MaterialPath);
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

	std::unordered_map<uint32_t, std::shared_ptr<VulkanDescriptorSet>>& Material::GetMaterialDescriptorSet()
	{
		return DescriptorSetManager::GetByName(m_MaterialPath);
	}

	const std::string& Material::GetShaderPath(const std::string& stage)
	{
		if (m_Shaders.find(stage) == m_Shaders.end())
		{
			std::stringstream ss;
			ss << "Material: " << m_MaterialPath << " : Not find sush shader: " << stage;

			SPIECS_CORE_WARN(ss.str());
		}

		return m_Shaders[stage];
	}

	void Material::BuildMaterial()
	{
		/**
		* @brief If ReBuild, need clear old descripotrset first.
		*/
		DescriptorSetManager::UnLoad(m_MaterialPath);

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
				SPIECS_CORE_ERROR("Material::BuildMaterial(): Invalid textureType.");
			}
		}

		/**
		* @brief Iter the constantParams for creating a host buffer and it's data memoryblock.
		*/
		m_ConstantParams.for_each([&](const std::string& k, const ConstantParam& v) {
			Int2 int2(v.set, v.binding);

			/**
			* @brief Create the key to map.
			*/
			m_Buffers[int2] = nullptr;

			/**
			* @brief Create the key to map, and add element to value.
			*/
			m_Buffermemoryblocks[int2].add_element(k, v.paramType);

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
				VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
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
		* @brief Iter m_Buffermemoryblocks for filling data on m_ConstantParams.
		*/
		for (auto& pair : m_Buffermemoryblocks)
		{
			/**
			* @brief Malloc the memory to this container.
			*/
			pair.second.build();

			/**
			* @brief Iter the block's element and filling data.
			*/
			pair.second.for_each([&](const std::string& name, void* pt) {

				/**
				* @brief Get constantParam by param's name.
				*/
				ConstantParam& ref = *m_ConstantParams.find_value(name);

				/**
				* @brief Fill in data to memory block.
				*/
				if (ref.paramType == "float4")
				{
					*reinterpret_cast<glm::vec4*>(pt) = std::any_cast<glm::vec4>(ref.paramValue);
				}
				else if (ref.paramType == "float3")
				{
					*reinterpret_cast<glm::vec4*>(pt) = glm::vec4(std::any_cast<glm::vec3>(ref.paramValue), 0.0f);
				}
				else if (ref.paramType == "float2")
				{
					*reinterpret_cast<glm::vec2*>(pt) = std::any_cast<glm::vec2>(ref.paramValue);
				}
				else if (ref.paramType == "float")
				{
					*reinterpret_cast<float*>(pt) = std::any_cast<float>(ref.paramValue);
				}
				return false;
			});

			/**
			* @brief Write the memeoryblock's data to m_Buffers.
			*/
			m_Buffers[pair.first]->WriteToBuffer(pair.second.get_addr());

			/**
			* @brief Flush the m_Buffers's memory.
			*/
			m_Buffers[pair.first]->Flush();
		}

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
		renderer->RegistyMaterial(m_MaterialPath, sv[1]);
	}
}