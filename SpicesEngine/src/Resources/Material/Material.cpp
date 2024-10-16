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
#include "Render/Renderer/DescriptorSetManager/BindLessTextureManager.h"
#include "Resources/Shader/Shader.h"

namespace Spices {

	Material::Material(const std::string& materialPath)
		: m_MaterialPath(materialPath)
	{
		SPICES_PROFILE_ZONE;

		/**
		* @breif Call Dserialize() while in Constructor.
		*/
		Deserialize();
	}

	Material::~Material()
	{
		SPICES_PROFILE_ZONE;

		/**
		* @brief UnLoad ShaderModule.
		*/
		for (auto& pair : m_Shaders)
		{
			for (int i = 0; i < pair.second.size(); i++)
			{
				std::stringstream ss;
				ss << pair.first << "." << pair.second[i];

				ResourcePool<Shader>::UnLoad(ss.str());
			}
		}

	}

	void Material::Serialize()
	{
		SPICES_PROFILE_ZONE;

		if (m_MaterialPath.empty())
		{
			SPICES_CORE_INFO("Please do not do that!");
		}
	}

	void Material::Deserialize()
	{
		SPICES_PROFILE_ZONE;

		if (m_MaterialPath.empty())
		{
			SPICES_CORE_WARN("Material::m_MaterialPath is empty.");
			return;
		}

		MaterialLoader::Load(m_MaterialPath, this);
	}

	const std::vector<std::string>& Material::GetShaderPath(const std::string& stage)
	{
		SPICES_PROFILE_ZONE;

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
		SPICES_PROFILE_ZONE;

		if(m_MaterialParameterBuffer == nullptr) return 0;
		else return m_MaterialParameterBuffer->GetAddress();
	}

	void Material::BuildMaterial(bool isAutoRegistry)
	{
		SPICES_PROFILE_ZONE;

		/**
		* @brief Registry ShaderModule.
		*/
		{
			SPICES_PROFILE_ZONEN("BuildMaterial::Registry ShaderModule");

			for (auto& pair : m_Shaders)
			{
				for (int i = 0; i < pair.second.size(); i++)
				{
					std::stringstream ss;
					ss << pair.first << "." << pair.second[i];

					ResourcePool<Shader>::Load<Shader>(ss.str(), pair.second[i], pair.first);
				}
			}
		}

		/**
		* @brief If ReBuild, need clear old descripotrset first.
		*/
		{
			SPICES_PROFILE_ZONEN("BuildMaterial::Clean Up old descripotrset");

			m_MaterialParameterBuffer  = nullptr;
			m_Buffermemoryblocks       = scl::runtime_memory_block();
		}

		/**
		* @brief Iter the constantParams and fill it's data to memoryblock.
		*/
		{
			SPICES_PROFILE_ZONEN("BuildMaterial::Build Local ConstantParameter Block");

			m_ConstantParams.for_each([&](const std::string& k, const ConstantParams& v) {
				m_Buffermemoryblocks.add_element(k, v.value.paramType);
				return false;
			});

			m_Buffermemoryblocks.build();
		}

		/**
		* @brief Return if not valid textureParameter or constantParameter.
		*/
		uint64_t size = m_TextureParams.size() * sizeof(unsigned int) + m_Buffermemoryblocks.get_bytes();
		if (size == 0)
		{
			std::vector<std::string> sv = StringLibrary::SplitString(m_MaterialPath, '.');
			auto renderer = RendererManager::GetRenderer(sv[0]);
			renderer->RegistryMaterial(m_MaterialPath, sv[1]);

			return;
		}

		/**
		* @brief Create Material Parameter to store all address and index.
		*/
		{
			SPICES_PROFILE_ZONEN("BuildMaterial::Create Material Parameter Buffer");

			m_MaterialParameterBuffer = std::make_unique<VulkanBuffer>(
				VulkanRenderBackend::GetState(),
				"MaterialParameterBuffer",
				size,
				VK_BUFFER_USAGE_STORAGE_BUFFER_BIT |
				VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT,
				VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT
			);
		}

		/**
		* @brief Registry texture to both ResourcePool, BindLessTextureManager, DescriptorSetManager and MaterialParameterBuffer.
		*/
		{
			SPICES_PROFILE_ZONEN("BuildMaterial::Registry texture");

			int tindex = 0;
			m_TextureParams.for_each([&](const std::string& k, TextureParam& v) {

				/**
				* @brief Only work with Texture2D now.
				* @todo more type support, reflection.
				*/
				if (v.textureType == "Texture2D")
				{
					std::shared_ptr<Texture> texture = ResourcePool<Texture>::Load<Texture2D>(v.texturePath, v.texturePath);
					v.index = BindLessTextureManager::Registry(v.texturePath);

					auto descriptorSet = DescriptorSetManager::Registry("PreRenderer", BINDLESS_TEXTURE_SET);

					/**
					* @brief Instance a VkWriteDescriptorSet.
					*/
					VkWriteDescriptorSet         write {};
					write.sType                = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
					write.dstBinding           = BINDLESS_TEXTURE_BINDING;
					write.dstSet               = descriptorSet->Get();
					write.descriptorType       = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
					write.pImageInfo           = texture->GetResource<VulkanImage>()->GetImageInfo();
					write.descriptorCount      = 1;
					write.dstArrayElement      = v.index;

					/**
					* @brief Update DescriptorSet.
					*/
					vkUpdateDescriptorSets(VulkanRenderBackend::GetState().m_Device, 1, &write, 0, nullptr);

					m_MaterialParameterBuffer->WriteToBuffer(&v.index, sizeof(unsigned int), tindex * sizeof(unsigned int));
					m_MaterialParameterBuffer->Flush();
				}

				/**
				* @brief Not supported format.
				*/
				else
				{
					SPICES_CORE_ERROR("Material::BuildMaterial(): Invalid textureType.");
				}

				tindex++;
				return false;
			});
		}

		/**
		* @brief Create ConstantParameter Buffer .
		*/
		{
			SPICES_PROFILE_ZONEN("BuildMaterial::Add ConstantParameter Buffer");

			m_Buffermemoryblocks.for_each([&](const std::string& name, void* pt) {
				ConstantParam& ref = m_ConstantParams.find_value(name)->value;
				size_t size = m_TextureParams.size() * sizeof(unsigned int) + m_Buffermemoryblocks.item_location(name);
				
				/**
				* @brief Fill in data to memory block.
				*/
				if      (ref.paramType == "float4")
				{
					*static_cast<glm::vec4*>(pt)     = std::any_cast<glm::vec4>(ref.paramValue);
					m_MaterialParameterBuffer->WriteToBuffer(pt, sizeof(glm::vec4), size);
					m_MaterialParameterBuffer->Flush();
				}
				else if (ref.paramType == "float3")
				{
					*static_cast<glm::vec3*>(pt)     = std::any_cast<glm::vec3>(ref.paramValue);
					m_MaterialParameterBuffer->WriteToBuffer(pt, sizeof(glm::vec3), size);
					m_MaterialParameterBuffer->Flush();
				}
				else if (ref.paramType == "float2")
				{
					*static_cast<glm::vec2*>(pt)     = std::any_cast<glm::vec2>(ref.paramValue);
					m_MaterialParameterBuffer->WriteToBuffer(pt, sizeof(glm::vec2), size);
					m_MaterialParameterBuffer->Flush();
				}
				else if (ref.paramType == "float")
				{
					*static_cast<float*>(pt)         = std::any_cast<float>(ref.paramValue);
					m_MaterialParameterBuffer->WriteToBuffer(pt, sizeof(float), size);
					m_MaterialParameterBuffer->Flush();
				}
				else if (ref.paramType == "int")
				{
					*static_cast<int*>(pt)           = std::any_cast<int>(ref.paramValue);
					m_MaterialParameterBuffer->WriteToBuffer(pt, sizeof(int), size);
					m_MaterialParameterBuffer->Flush();
				}
				else if (ref.paramType == "bool")
				{
					*static_cast<bool*>(pt)          = std::any_cast<bool>(ref.paramValue);
					m_MaterialParameterBuffer->WriteToBuffer(pt, sizeof(bool), size);
					m_MaterialParameterBuffer->Flush();
				}
				else
				{
					SPICES_CORE_ERROR("Material::BuildMaterial(): Invalid paramType.");
				}
	
				return false;
			});
		}

		/**
		* @brief Create PipelineLayout.
		*/
		if (isAutoRegistry)
		{
			std::vector<std::string> sv = StringLibrary::SplitString(m_MaterialPath, '.');
			auto renderer = RendererManager::GetRenderer(sv[0]);
			renderer->RegistryMaterial(m_MaterialPath, sv[1]);
		}
	}

	void Material::UpdateMaterial()
	{
		SPICES_PROFILE_ZONE;

		/**
		* @brief Iter the constantParams and fill it's data to memoryblock.
		*/
		{
			SPICES_PROFILE_ZONEN("BuildMaterial::Build Local ConstantParameter Block");

			m_Buffermemoryblocks = scl::runtime_memory_block();

			m_ConstantParams.for_each([&](const std::string& k, const ConstantParams& v) {
				m_Buffermemoryblocks.add_element(k, v.value.paramType);
				return false;
			});

			m_Buffermemoryblocks.build();
		}

		/**
		* @brief Registry texture to both ResourcePool, BindLessTextureManager, DescriptorSetManager and MaterialParameterBuffer.
		*/
		{
			SPICES_PROFILE_ZONEN("BuildMaterial::Registry texture");

			int tindex = 0;
			m_TextureParams.for_each([&](const std::string& k, TextureParam& v) {

				/**
				* @brief Only work with Texture2D now.
				* @todo more type support, reflection.
				*/
				if (v.textureType == "Texture2D")
				{
					std::shared_ptr<Texture> texture = ResourcePool<Texture>::Load<Texture2D>(v.texturePath, v.texturePath);
					v.index = BindLessTextureManager::Registry(v.texturePath);

					auto descriptorSet = DescriptorSetManager::Registry("PreRenderer", BINDLESS_TEXTURE_SET);

					/**
					* @brief Instance a VkWriteDescriptorSet.
					*/
					VkWriteDescriptorSet         write {};
					write.sType                = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
					write.dstBinding           = BINDLESS_TEXTURE_BINDING;
					write.dstSet               = descriptorSet->Get();
					write.descriptorType       = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
					write.pImageInfo           = texture->GetResource<VulkanImage>()->GetImageInfo();
					write.descriptorCount      = 1;
					write.dstArrayElement      = v.index;

					/**
					* @brief Update DescriptorSet.
					*/
					vkUpdateDescriptorSets(VulkanRenderBackend::GetState().m_Device, 1, &write, 0, nullptr);

					m_MaterialParameterBuffer->WriteToBuffer(&v.index, sizeof(unsigned int), tindex * sizeof(unsigned int));
					m_MaterialParameterBuffer->Flush();
				}

				/**
				* @brief Not supported format.
				*/
				else
				{
					SPICES_CORE_ERROR("Material::BuildMaterial(): Invalid textureType.");
				}

				tindex++;
				return false;
			});
		}

		/**
		* @brief Create ConstantParameter Buffer .
		*/
		{
			SPICES_PROFILE_ZONEN("BuildMaterial::Add ConstantParameter Buffer");

			m_Buffermemoryblocks.for_each([&](const std::string& name, void* pt) {
				ConstantParam& ref = m_ConstantParams.find_value(name)->value;
				size_t size = m_TextureParams.size() * sizeof(unsigned int) + m_Buffermemoryblocks.item_location(name);
				
				/**
				* @brief Fill in data to memory block.
				*/
				if      (ref.paramType == "float4")
				{
					*static_cast<glm::vec4*>(pt)     = std::any_cast<glm::vec4>(ref.paramValue);
					m_MaterialParameterBuffer->WriteToBuffer(pt, sizeof(glm::vec4), size);
					m_MaterialParameterBuffer->Flush();
				}
				else if (ref.paramType == "float3")
				{
					*static_cast<glm::vec3*>(pt)     = std::any_cast<glm::vec3>(ref.paramValue);
					m_MaterialParameterBuffer->WriteToBuffer(pt, sizeof(glm::vec3), size);
					m_MaterialParameterBuffer->Flush();
				}
				else if (ref.paramType == "float2")
				{
					*static_cast<glm::vec2*>(pt)     = std::any_cast<glm::vec2>(ref.paramValue);
					m_MaterialParameterBuffer->WriteToBuffer(pt, sizeof(glm::vec2), size);
					m_MaterialParameterBuffer->Flush();
				}
				else if (ref.paramType == "float")
				{
					*static_cast<float*>(pt)         = std::any_cast<float>(ref.paramValue);
					m_MaterialParameterBuffer->WriteToBuffer(pt, sizeof(float), size);
					m_MaterialParameterBuffer->Flush();
				}
				else if (ref.paramType == "int")
				{
					*static_cast<int*>(pt)           = std::any_cast<int>(ref.paramValue);
					m_MaterialParameterBuffer->WriteToBuffer(pt, sizeof(int), size);
					m_MaterialParameterBuffer->Flush();
				}
				else if (ref.paramType == "bool")
				{
					*static_cast<bool*>(pt)           = std::any_cast<bool>(ref.paramValue);
					m_MaterialParameterBuffer->WriteToBuffer(pt, sizeof(bool), size);
					m_MaterialParameterBuffer->Flush();
				}
				else
				{
					SPICES_CORE_ERROR("Material::BuildMaterial(): Invalid paramType.");
				}
	
				return false;
			});
		}
	}
}