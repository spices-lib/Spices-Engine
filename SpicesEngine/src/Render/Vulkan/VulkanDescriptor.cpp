/**
* @file VulkanDescriptor.cpp.
* @brief The VulkanDescriptorPool and VulkanDescriptorSet Class Implementation.
* @author Spices.
*/

#include "Pchheader.h"
#include "VulkanDescriptor.h"
#include "..\..\..\assets\Shaders\src\Header\ShaderCommon.h"

namespace Spices {
	
	VulkanDescriptorPool::Builder& 
	VulkanDescriptorPool::Builder::
	AddPoolSize(
		VkDescriptorType descriptorType , 
		uint32_t         count
	)
	{
		SPICES_PROFILE_ZONE;

		m_PoolSizes.push_back({ descriptorType, count });
		m_MaxSets += count;
		return *this;
	}

	VulkanDescriptorPool::Builder& 
	VulkanDescriptorPool::Builder::
	SetPoolFlags(
		VkDescriptorPoolCreateFlags flags
	)
	{
		SPICES_PROFILE_ZONE;

		m_PoolFlags = flags;
		return *this;
	}

	std::shared_ptr<VulkanDescriptorPool> 
	VulkanDescriptorPool::Builder::
	Build(VulkanState& vulkanState) const
	{
		SPICES_PROFILE_ZONE;

		/**
		* @brief Instance a VulkanDescriptorPool.
		*/
		return std::make_shared<VulkanDescriptorPool>(vulkanState, m_MaxSets, m_PoolFlags, m_PoolSizes);
	}

	VulkanDescriptorPool::VulkanDescriptorPool(
		VulkanState&                             vulkanState , 
		uint32_t                                 maxSets     , 
		VkDescriptorPoolCreateFlags              poolFlags   , 
		const std::vector<VkDescriptorPoolSize>& poolSizes
	)
		: VulkanObject(vulkanState)
	{
		SPICES_PROFILE_ZONE;

		/**
		* @brief Instance a VkDescriptorPoolCreateInfo.
		*/
		VkDescriptorPoolCreateInfo descriptorPoolInfo{};
		descriptorPoolInfo.sType           = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
		descriptorPoolInfo.poolSizeCount   = static_cast<uint32_t>(poolSizes.size());
		descriptorPoolInfo.pPoolSizes      = poolSizes.data();
		descriptorPoolInfo.maxSets         = maxSets;
		descriptorPoolInfo.flags           = poolFlags;

		/**
		* @brief Create a VkDescriptorPool.
		*/
		VK_CHECK(vkCreateDescriptorPool(vulkanState.m_Device, &descriptorPoolInfo, nullptr, &m_DescriptorPool));
		VulkanDebugUtils::SetObjectName(VK_OBJECT_TYPE_DESCRIPTOR_POOL, m_DescriptorPool, vulkanState.m_Device, "SpicesEngineDescriptorPool");
	}

	VulkanDescriptorPool::~VulkanDescriptorPool()
	{
		SPICES_PROFILE_ZONE;

		/**
		* @brief Destroy DescriptorPool.
		*/
		vkDestroyDescriptorPool(m_VulkanState.m_Device, m_DescriptorPool, nullptr);
	}

	void VulkanDescriptorPool::resetPool()
	{
		SPICES_PROFILE_ZONE;

		/**
		* @brief Reset DescriptorPool.
		*/
		vkResetDescriptorPool(m_VulkanState.m_Device, m_DescriptorPool, 0);
	}
	
	VulkanDescriptorSetLayout::~VulkanDescriptorSetLayout()
	{
		SPICES_PROFILE_ZONE;

		/**
		* @brief Destroy DescriptorSetLayout.
		*/
		vkDestroyDescriptorSetLayout(m_VulkanState.m_Device, m_Layout, nullptr);
	}

	void VulkanDescriptorSetLayout::BuildDescriptorSetLayout(
		const std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding>& bindings,
		const std::string&                                                caption
	)
	{
		SPICES_PROFILE_ZONE;

		/**
		* @brief Get linear binding data and flags.
		*/
		std::vector<VkDescriptorSetLayoutBinding> setLayoutBindings{};
		std::vector<VkDescriptorBindingFlags>     setBindingFlags{};

		for (auto& kv : bindings) 
		{
			setLayoutBindings.push_back(kv.second);

			switch (kv.second.descriptorType)
			{
			case VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT:
				setBindingFlags.push_back(VK_DESCRIPTOR_BINDING_PARTIALLY_BOUND_BIT); /* @brief VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT not support VK_DESCRIPTOR_BINDING_UPDATE_AFTER_BIND_BIT. */
				break;
			default:
				setBindingFlags.push_back(VK_DESCRIPTOR_BINDING_PARTIALLY_BOUND_BIT | VK_DESCRIPTOR_BINDING_UPDATE_AFTER_BIND_BIT);
				break;
			}
		}

		/**
		* @breif Instance a VkDescriptorSetLayoutBindingFlagsCreateInfo.
		*/
		VkDescriptorSetLayoutBindingFlagsCreateInfo       bindingFlags{};
		bindingFlags.sType                              = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_BINDING_FLAGS_CREATE_INFO;
		bindingFlags.pNext                              = nullptr;
		bindingFlags.pBindingFlags                      = setBindingFlags.data();
		bindingFlags.bindingCount                       = static_cast<uint32_t>(setBindingFlags.size());

		/**
		* @breif Instance a VkDescriptorSetLayoutCreateInfo.
		*/
		VkDescriptorSetLayoutCreateInfo                   descriptorSetLayoutCreateInfo{};
		descriptorSetLayoutCreateInfo.sType             = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
		descriptorSetLayoutCreateInfo.bindingCount      = static_cast<uint32_t>(setLayoutBindings.size());
		descriptorSetLayoutCreateInfo.pBindings         = setLayoutBindings.data();
		descriptorSetLayoutCreateInfo.flags             = VK_DESCRIPTOR_SET_LAYOUT_CREATE_UPDATE_AFTER_BIND_POOL_BIT;  /* @brief Create if from a descriptor pool that has update after bind. */
		descriptorSetLayoutCreateInfo.pNext             = &bindingFlags;

		/**
		* @brief Create DescriptorSetLayout.
		*/
		VK_CHECK(vkCreateDescriptorSetLayout(m_VulkanState.m_Device, &descriptorSetLayoutCreateInfo, nullptr, &m_Layout))
		VulkanDebugUtils::SetObjectName(VK_OBJECT_TYPE_DESCRIPTOR_SET_LAYOUT, m_Layout, m_VulkanState.m_Device, "DescriptorSetLayout" + caption);
	}

	void VulkanDescriptorSetLayout::BuildBindLessTextureDescriptorSetLayout(
		const std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding>& bindings , 
		const std::string&                                                caption
	)
	{
		SPICES_PROFILE_ZONE;

		/**
		* @brief Get linear binding data and flags.
		*/
		std::vector<VkDescriptorSetLayoutBinding> setLayoutBindings{};
		std::vector<VkDescriptorBindingFlags>     setBindingFlags{};

		for (auto& kv : bindings) 
		{
			setLayoutBindings.push_back(kv.second);

			switch (kv.second.descriptorType)
			{
			case VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER:
				setBindingFlags.push_back(VK_DESCRIPTOR_BINDING_PARTIALLY_BOUND_BIT | VK_DESCRIPTOR_BINDING_VARIABLE_DESCRIPTOR_COUNT_BIT | VK_DESCRIPTOR_BINDING_UPDATE_AFTER_BIND_BIT);
				break;
			default:
				SPICES_CORE_ERROR("BindLess only support COMBINED_IMAGE_SAMPLER type binding");
				break;
			}
		}

		/**
		* @breif Instance a VkDescriptorSetLayoutBindingFlagsCreateInfo.
		*/
		VkDescriptorSetLayoutBindingFlagsCreateInfo       bindingFlags{};
		bindingFlags.sType                              = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_BINDING_FLAGS_CREATE_INFO;
		bindingFlags.pNext                              = nullptr;
		bindingFlags.pBindingFlags                      = setBindingFlags.data();
		bindingFlags.bindingCount                       = static_cast<uint32_t>(setBindingFlags.size());

		/**
		* @breif Instance a VkDescriptorSetLayoutCreateInfo.
		*/
		VkDescriptorSetLayoutCreateInfo                   descriptorSetLayoutCreateInfo{};
		descriptorSetLayoutCreateInfo.sType             = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
		descriptorSetLayoutCreateInfo.bindingCount      = static_cast<uint32_t>(setLayoutBindings.size());
		descriptorSetLayoutCreateInfo.pBindings         = setLayoutBindings.data();
		descriptorSetLayoutCreateInfo.flags             = VK_DESCRIPTOR_SET_LAYOUT_CREATE_UPDATE_AFTER_BIND_POOL_BIT;  /* @brief Create if from a descriptor pool that has update after bind. */
		descriptorSetLayoutCreateInfo.pNext             = &bindingFlags;

		/**
		* @brief Create DescriptorSetLayout.
		*/
		VK_CHECK(vkCreateDescriptorSetLayout(m_VulkanState.m_Device, &descriptorSetLayoutCreateInfo, nullptr, &m_Layout))
		VulkanDebugUtils::SetObjectName(VK_OBJECT_TYPE_DESCRIPTOR_SET_LAYOUT, m_Layout, m_VulkanState.m_Device, "DescriptorSetLayout" + caption);
	}

	VulkanDescriptorSet::~VulkanDescriptorSet()
	{
		SPICES_PROFILE_ZONE;

		/**
		* @brief Free DescriptorSet.
		*/
		vkFreeDescriptorSets(m_VulkanState.m_Device, m_Pool->GetPool(), 1, &m_DescriptorSet);
	}

	void VulkanDescriptorSet::AddBinding(
		uint32_t            binding          ,
		VkDescriptorType    descriptorType   ,
		VkShaderStageFlags  stageFlags       ,
		uint32_t            count
	)
	{
		SPICES_PROFILE_ZONE;

		/**
		* @brief Instance a VkDescriptorSetLayoutBinding.
		*/
		VkDescriptorSetLayoutBinding layoutBinding{};
		layoutBinding.binding          = binding;
		layoutBinding.descriptorType   = descriptorType;
		layoutBinding.descriptorCount  = count;
		layoutBinding.stageFlags       = stageFlags;
		
		m_Bindings[binding]            = layoutBinding;
	}

	void VulkanDescriptorSet::BuildDescriptorSet(const std::string& createrName)
	{
		SPICES_PROFILE_ZONE;

		/**
		* @brief Build DescriptorSetLayout.
		*/
		m_Layout.BuildDescriptorSetLayout(m_Bindings, createrName);

		/**
		* @brief Instance a VkDescriptorSetAllocateInfo.
		*/
		VkDescriptorSetAllocateInfo      allocInfo{};
		allocInfo.sType                = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
		allocInfo.descriptorPool       = m_Pool->GetPool();
		allocInfo.pSetLayouts          = &m_Layout.Get();
		allocInfo.descriptorSetCount   = 1;
		
		/**
		* @brief Allocate DescriptorSet.
		*/
		VK_CHECK(vkAllocateDescriptorSets(m_VulkanState.m_Device, &allocInfo, &m_DescriptorSet))
		VulkanDebugUtils::SetObjectName(VK_OBJECT_TYPE_DESCRIPTOR_SET, m_DescriptorSet, m_VulkanState.m_Device, "DescriptorSet" + createrName);
	}

	void VulkanDescriptorSet::BuildBindLessTextureDescriptorSet(const std::string& createrName)
	{
		SPICES_PROFILE_ZONE;

		/**
		* @brief Build DescriptorSetLayout.
		*/
		m_Layout.BuildBindLessTextureDescriptorSetLayout(m_Bindings, createrName);

		/**
		* @brief Instance a VkDescriptorSetVariableDescriptorCountAllocateInfo.
		*/
		uint32_t maxBinding = BINDLESS_TEXTURE_MAXNUM;
		VkDescriptorSetVariableDescriptorCountAllocateInfo     countInfo{};
		countInfo.sType                                      = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_VARIABLE_DESCRIPTOR_COUNT_ALLOCATE_INFO;
		countInfo.descriptorSetCount                         = 1;
		countInfo.pDescriptorCounts                          = &maxBinding;

		/**
		* @brief Instance a VkDescriptorSetAllocateInfo.
		*/
		VkDescriptorSetAllocateInfo      allocInfo{};
		allocInfo.sType                = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
		allocInfo.descriptorPool       = m_Pool->GetPool();
		allocInfo.pSetLayouts          = &m_Layout.Get();
		allocInfo.descriptorSetCount   = 1;
		allocInfo.pNext                = &countInfo;
		
		/**
		* @brief Allocate DescriptorSet.
		*/
		VK_CHECK(vkAllocateDescriptorSets(m_VulkanState.m_Device, &allocInfo, &m_DescriptorSet))
		VulkanDebugUtils::SetObjectName(VK_OBJECT_TYPE_DESCRIPTOR_SET, m_DescriptorSet, m_VulkanState.m_Device, "DescriptorSet" + createrName);
	}

	void VulkanDescriptorSet::UpdateDescriptorSet(
		ImageInfo&                        imageInfo  ,
		BufferInfo&                       bufferInfo ,
		const VkAccelerationStructureKHR& accel
	) const
	{
		SPICES_PROFILE_ZONE;

		for(auto& pair : m_Bindings)
		{
			/**
			* @brief Instance a VkWriteDescriptorSet.
			*/
			VkWriteDescriptorSet write{};
			write.sType                 = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
			write.dstBinding            = pair.first;
			write.dstSet                = m_DescriptorSet;
			write.descriptorType        = pair.second.descriptorType;

			switch(write.descriptorType)
			{
			case VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT:
			case VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER:
			case VK_DESCRIPTOR_TYPE_STORAGE_IMAGE:
				write.pImageInfo        = imageInfo[pair.first].data();
				write.descriptorCount   = static_cast<uint32_t>(imageInfo[pair.first].size());
				break;
			case VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER:
			case VK_DESCRIPTOR_TYPE_STORAGE_BUFFER:
				write.pBufferInfo       = &bufferInfo[pair.first];
				write.descriptorCount   = 1;
			case VK_DESCRIPTOR_TYPE_ACCELERATION_STRUCTURE_KHR:
				VkWriteDescriptorSetAccelerationStructureKHR descASInfo {};
				descASInfo.sType                          = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET_ACCELERATION_STRUCTURE_KHR;
				descASInfo.accelerationStructureCount     = 1;
				descASInfo.pAccelerationStructures        = &accel;

				write.pNext             = &descASInfo;
				write.descriptorCount   = 1;
				break;
			}
			
			/**
			* @brief Update DescriptorSet.
			*/
			vkUpdateDescriptorSets(m_VulkanState.m_Device, 1, &write, 0, nullptr);
		}
	}

	void VulkanDescriptorSet::UpdateDescriptorSet(BufferInfo& bufferInfo) const
	{
		SPICES_PROFILE_ZONE;

		for (auto& pair : m_Bindings)
		{
			/**
			* @brief Instance a VkWriteDescriptorSet.
			*/
			VkWriteDescriptorSet         write {};
			write.sType                = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
			write.dstBinding           = pair.first;
			write.dstSet               = m_DescriptorSet;
			write.descriptorType       = pair.second.descriptorType;
			write.pBufferInfo          = &bufferInfo[pair.first];
			write.descriptorCount      = 1;

			/**
			* @brief Update DescriptorSet.
			*/
			vkUpdateDescriptorSets(m_VulkanState.m_Device, 1, &write, 0, nullptr);
		}
	}

	void VulkanDescriptorSet::UpdateDescriptorSet(ImageInfo& imageInfo) const
	{
		SPICES_PROFILE_ZONE;

		for (auto& pair : m_Bindings)
		{
			/**
			* @brief Instance a VkWriteDescriptorSet.
			*/
			VkWriteDescriptorSet         write {};
			write.sType                = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
			write.dstBinding           = pair.first;
			write.dstSet               = m_DescriptorSet;
			write.descriptorType       = pair.second.descriptorType;
			write.pImageInfo           = imageInfo[pair.first].data();
			write.descriptorCount      = static_cast<uint32_t>(imageInfo[pair.first].size());

			/**
			* @brief Update DescriptorSet.
			*/
			vkUpdateDescriptorSets(m_VulkanState.m_Device, 1, &write, 0, nullptr);
		}
	}

	void VulkanDescriptorSet::UpdateBindLessTextureDescriptorSet(ImageInfo& imageInfo) const
	{
		SPICES_PROFILE_ZONE;

		for (auto& pair : m_Bindings)
		{
			/**
			* @brief Instance a VkWriteDescriptorSet.
			*/
			VkWriteDescriptorSet         write {};
			write.sType                = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
			write.dstBinding           = pair.first;
			write.dstSet               = m_DescriptorSet;
			write.descriptorType       = pair.second.descriptorType;
			write.pImageInfo           = imageInfo[pair.first].data();
			write.descriptorCount      = static_cast<uint32_t>(imageInfo[pair.first].size());
			write.dstArrayElement      = 10;

			/**
			* @brief Update DescriptorSet.
			*/
			vkUpdateDescriptorSets(m_VulkanState.m_Device, 1, &write, 0, nullptr);
		}
	}
}
