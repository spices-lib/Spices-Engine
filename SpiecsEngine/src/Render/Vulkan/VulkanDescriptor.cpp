#include "Pchheader.h"
#include "VulkanDescriptor.h"

namespace Spiecs {
	
	VulkanDescriptorPool::Builder& VulkanDescriptorPool::Builder::AddPoolSize(VkDescriptorType descriptorType, uint32_t count)
	{
		m_PoolSizes.push_back({ descriptorType, count });
		return *this;
	}

	VulkanDescriptorPool::Builder& VulkanDescriptorPool::Builder::SetPoolFlags(VkDescriptorPoolCreateFlags flags)
	{
		m_PoolFlags = flags;
		return *this;
	}

	VulkanDescriptorPool::Builder& VulkanDescriptorPool::Builder::SetMaxSets(uint32_t count)
	{
		m_MaxSets = count;
		return *this;
	}

	std::shared_ptr<VulkanDescriptorPool> VulkanDescriptorPool::Builder::Build(VulkanState& vulkanState) const
	{
		return std::make_shared<VulkanDescriptorPool>(vulkanState, m_MaxSets, m_PoolFlags, m_PoolSizes);
	}

	VulkanDescriptorPool::VulkanDescriptorPool(VulkanState& vulkanState, uint32_t maxSets, VkDescriptorPoolCreateFlags poolFlags, const std::vector<VkDescriptorPoolSize>& poolSizes)
		: VulkanObject(vulkanState)
	{
		VkDescriptorPoolCreateInfo descriptorPoolInfo{};
		descriptorPoolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
		descriptorPoolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
		descriptorPoolInfo.pPoolSizes = poolSizes.data();
		descriptorPoolInfo.maxSets = maxSets;
		descriptorPoolInfo.flags = poolFlags;

		if (vkCreateDescriptorPool(vulkanState.m_Device, &descriptorPoolInfo, nullptr, &m_DescriptorPool) !=
			VK_SUCCESS) {
			throw std::runtime_error("failed to create descriptor pool!");
		}
	}

	VulkanDescriptorPool::~VulkanDescriptorPool()
	{
		vkDestroyDescriptorPool(m_VulkanState.m_Device, m_DescriptorPool, nullptr);
	}

	void VulkanDescriptorPool::resetPool()
	{
		vkResetDescriptorPool(m_VulkanState.m_Device, m_DescriptorPool, 0);
	}
	
	VulkanDescriptorSetLayout::~VulkanDescriptorSetLayout()
	{
		vkDestroyDescriptorSetLayout(m_VulkanState.m_Device, m_Layout, nullptr);
	}

	void VulkanDescriptorSetLayout::BuildDescriptorSetLayout(
		const std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding>& bindings)
	{
		std::vector<VkDescriptorSetLayoutBinding> setLayoutBindings{};
		for (auto& kv : bindings) {
			setLayoutBindings.push_back(kv.second);
		}
		
		VkDescriptorSetLayoutCreateInfo descriptorSetLayoutCreateInfo{};
		descriptorSetLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
		descriptorSetLayoutCreateInfo.bindingCount = static_cast<uint32_t>(setLayoutBindings.size());
		descriptorSetLayoutCreateInfo.pBindings = setLayoutBindings.data();

		VK_CHECK(vkCreateDescriptorSetLayout(m_VulkanState.m_Device, &descriptorSetLayoutCreateInfo, nullptr, &m_Layout));
	}

	VulkanDescriptorSet::~VulkanDescriptorSet()
	{
		vkFreeDescriptorSets(m_VulkanState.m_Device, m_Pool->GetPool(), 1, &m_DescriptorSet);
	}

	void VulkanDescriptorSet::AddBinding(
		uint32_t            binding,
		VkDescriptorType    descriptorType,
		VkShaderStageFlags  stageFlags,
		uint32_t            count
	)
	{
		VkDescriptorSetLayoutBinding layoutBinding{};
		layoutBinding.binding          = binding;
		layoutBinding.descriptorType   = descriptorType;
		layoutBinding.descriptorCount  = count;
		layoutBinding.stageFlags       = stageFlags;
		
		m_Bindings[binding]            = layoutBinding;
	}

	void VulkanDescriptorSet::BuildDescriptorSet()
	{
		m_Layout.BuildDescriptorSetLayout(m_Bindings);

		VkDescriptorSetAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
		allocInfo.descriptorPool = m_Pool->GetPool();
		allocInfo.pSetLayouts = &m_Layout.Get();
		allocInfo.descriptorSetCount = 1;
			
		VK_CHECK(vkAllocateDescriptorSets(m_VulkanState.m_Device, &allocInfo, &m_DescriptorSet));
	}

	void VulkanDescriptorSet::UpdateDescriptorSet(
		ImageInfo&  imageInfo,
		BufferInfo& bufferInfo
	)
	{
		for(auto& pair : m_Bindings)
		{
			VkWriteDescriptorSet write{};
			write.sType            = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
			write.dstBinding       = pair.first;
			write.dstSet           = m_DescriptorSet;
			write.descriptorType   = pair.second.descriptorType;

			switch(write.descriptorType)
			{
			case VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT:
			case VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER:
				write.pImageInfo        = imageInfo[pair.first].data();
				write.descriptorCount   = static_cast<uint32_t>(imageInfo[pair.first].size());
				break;
			case VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER:
				write.pBufferInfo       = &bufferInfo[pair.first];
				write.descriptorCount   = 1;
				break;
			}
			
			vkUpdateDescriptorSets(m_VulkanState.m_Device, 1, &write, 0, nullptr);
		}
	}

	void VulkanDescriptorSet::UpdateDescriptorSet(BufferInfo& bufferInfo)
	{
		for (auto& pair : m_Bindings)
		{
			VkWriteDescriptorSet write{};
			write.sType           = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
			write.dstBinding      = pair.first;
			write.dstSet          = m_DescriptorSet;
			write.descriptorType  = pair.second.descriptorType;
			write.pBufferInfo     = &bufferInfo[pair.first];
			write.descriptorCount = 1;

			vkUpdateDescriptorSets(m_VulkanState.m_Device, 1, &write, 0, nullptr);
		}
	}

	void VulkanDescriptorSet::UpdateDescriptorSet(ImageInfo& imageInfo)
	{
		for (auto& pair : m_Bindings)
		{
			VkWriteDescriptorSet write{};
			write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
			write.dstBinding = pair.first;
			write.dstSet = m_DescriptorSet;
			write.descriptorType = pair.second.descriptorType;
			write.pImageInfo = imageInfo[pair.first].data();
			write.descriptorCount = static_cast<uint32_t>(imageInfo[pair.first].size());

			vkUpdateDescriptorSets(m_VulkanState.m_Device, 1, &write, 0, nullptr);
		}
	}
}
