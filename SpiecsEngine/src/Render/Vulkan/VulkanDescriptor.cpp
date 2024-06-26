/**
* @file VulkanDescriptor.cpp.
* @brief The VulkanDescriptorPool and VulkanDescriptorSet Class Implementation.
* @author Spiecs.
*/

#include "Pchheader.h"
#include "VulkanDescriptor.h"

namespace Spiecs {
	
	VulkanDescriptorPool::Builder& 
	VulkanDescriptorPool::Builder::
	AddPoolSize(
		VkDescriptorType descriptorType , 
		uint32_t         count
	)
	{
		SPIECS_PROFILE_ZONE;

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
		SPIECS_PROFILE_ZONE;

		m_PoolFlags = flags;
		return *this;
	}

	std::shared_ptr<VulkanDescriptorPool> 
	VulkanDescriptorPool::Builder::
	Build(VulkanState& vulkanState) const
	{
		SPIECS_PROFILE_ZONE;

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
		SPIECS_PROFILE_ZONE;

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
		VulkanDebugUtils::SetObjectName(VK_OBJECT_TYPE_DESCRIPTOR_POOL, m_DescriptorPool, vulkanState.m_Device, "SpiecsEngineDescriptorPool");
	}

	VulkanDescriptorPool::~VulkanDescriptorPool()
	{
		SPIECS_PROFILE_ZONE;

		/**
		* @brief Destroy DescriptorPool.
		*/
		vkDestroyDescriptorPool(m_VulkanState.m_Device, m_DescriptorPool, nullptr);
	}

	void VulkanDescriptorPool::resetPool()
	{
		SPIECS_PROFILE_ZONE;

		/**
		* @brief Reset DescriptorPool.
		*/
		vkResetDescriptorPool(m_VulkanState.m_Device, m_DescriptorPool, 0);
	}
	
	VulkanDescriptorSetLayout::~VulkanDescriptorSetLayout()
	{
		SPIECS_PROFILE_ZONE;

		/**
		* @brief Destroy DescriptorSetLayout.
		*/
		vkDestroyDescriptorSetLayout(m_VulkanState.m_Device, m_Layout, nullptr);
	}

	void VulkanDescriptorSetLayout::BuildDescriptorSetLayout(
		const std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding>& bindings
	)
	{
		SPIECS_PROFILE_ZONE;

		/**
		* @brief Get linear binding data.
		*/
		std::vector<VkDescriptorSetLayoutBinding> setLayoutBindings{};
		for (auto& kv : bindings) 
		{
			setLayoutBindings.push_back(kv.second);
		}
		
		/**
		* @breif Instance a VkDescriptorSetLayoutCreateInfo.
		*/
		VkDescriptorSetLayoutCreateInfo descriptorSetLayoutCreateInfo{};
		descriptorSetLayoutCreateInfo.sType             = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
		descriptorSetLayoutCreateInfo.bindingCount      = static_cast<uint32_t>(setLayoutBindings.size());
		descriptorSetLayoutCreateInfo.pBindings         = setLayoutBindings.data();

		/**
		* @brief Create DescriptorSetLayout.
		*/
		VK_CHECK(vkCreateDescriptorSetLayout(m_VulkanState.m_Device, &descriptorSetLayoutCreateInfo, nullptr, &m_Layout));
		VulkanDebugUtils::SetObjectName(VK_OBJECT_TYPE_DESCRIPTOR_SET_LAYOUT, m_Layout, m_VulkanState.m_Device, "DescriptorSetLayout");
	}

	VulkanDescriptorSet::~VulkanDescriptorSet()
	{
		SPIECS_PROFILE_ZONE;

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
		SPIECS_PROFILE_ZONE;

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
		SPIECS_PROFILE_ZONE;

		/**
		* @brief Build DescriptorSetLayout.
		*/
		m_Layout.BuildDescriptorSetLayout(m_Bindings);

		/**
		* @brief Instance a VkDescriptorSetAllocateInfo.
		*/
		VkDescriptorSetAllocateInfo allocInfo{};
		allocInfo.sType                = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
		allocInfo.descriptorPool       = m_Pool->GetPool();
		allocInfo.pSetLayouts          = &m_Layout.Get();
		allocInfo.descriptorSetCount   = 1;

		/**
		* @brief Allocate DescriptorSet.
		*/
		VK_CHECK(vkAllocateDescriptorSets(m_VulkanState.m_Device, &allocInfo, &m_DescriptorSet));
		VulkanDebugUtils::SetObjectName(VK_OBJECT_TYPE_DESCRIPTOR_SET, m_DescriptorSet, m_VulkanState.m_Device, createrName);
	}

	void VulkanDescriptorSet::UpdateDescriptorSet(
		ImageInfo&                 imageInfo  ,
		BufferInfo&                bufferInfo ,
		VkAccelerationStructureKHR accel
	)
	{
		SPIECS_PROFILE_ZONE;

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

	void VulkanDescriptorSet::UpdateDescriptorSet(BufferInfo& bufferInfo)
	{
		SPIECS_PROFILE_ZONE;

		for (auto& pair : m_Bindings)
		{
			/**
			* @brief Instance a VkWriteDescriptorSet.
			*/
			VkWriteDescriptorSet write{};
			write.sType           = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
			write.dstBinding      = pair.first;
			write.dstSet          = m_DescriptorSet;
			write.descriptorType  = pair.second.descriptorType;
			write.pBufferInfo     = &bufferInfo[pair.first];
			write.descriptorCount = 1;

			/**
			* @brief Update DescriptorSet.
			*/
			vkUpdateDescriptorSets(m_VulkanState.m_Device, 1, &write, 0, nullptr);
		}
	}

	void VulkanDescriptorSet::UpdateDescriptorSet(ImageInfo& imageInfo)
	{
		SPIECS_PROFILE_ZONE;

		for (auto& pair : m_Bindings)
		{
			/**
			* @brief Instance a VkWriteDescriptorSet.
			*/
			VkWriteDescriptorSet write{};
			write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
			write.dstBinding = pair.first;
			write.dstSet = m_DescriptorSet;
			write.descriptorType = pair.second.descriptorType;
			write.pImageInfo = imageInfo[pair.first].data();
			write.descriptorCount = static_cast<uint32_t>(imageInfo[pair.first].size());

			/**
			* @brief Update DescriptorSet.
			*/
			vkUpdateDescriptorSets(m_VulkanState.m_Device, 1, &write, 0, nullptr);
		}
	}
}
