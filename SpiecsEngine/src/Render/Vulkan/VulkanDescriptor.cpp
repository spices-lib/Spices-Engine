#include "pchheader.h"
#include "VulkanDescriptor.h"

namespace Spiecs {

	VulkanDescriptorSetLayout::Builder::Builder(VulkanDescriptorSetLayout* oldLayout)
	{
		if (oldLayout)
		{
			m_Bindings = oldLayout->m_Bindings;
		}
	}

	VulkanDescriptorSetLayout::Builder& VulkanDescriptorSetLayout::Builder::AddBinding(uint32_t binding, VkDescriptorType descriptorType, VkShaderStageFlags stageFlags, uint32_t count)
	{
		assert(m_Bindings.count(binding) == 0 && "Binding already in use");
		VkDescriptorSetLayoutBinding layoutBinding{};
		layoutBinding.binding = binding;
		layoutBinding.descriptorType = descriptorType;
		layoutBinding.descriptorCount = count;
		layoutBinding.stageFlags = stageFlags;
		m_Bindings[binding] = layoutBinding;
		return *this;
	}

	std::unique_ptr<VulkanDescriptorSetLayout> VulkanDescriptorSetLayout::Builder::Build(VulkanState& vulkanState) const
	{
		return std::make_unique<VulkanDescriptorSetLayout>(vulkanState, m_Bindings);
	}

	VulkanDescriptorSetLayout::VulkanDescriptorSetLayout(VulkanState& vulkanState, std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> bindings)
		: VulkanObject(vulkanState)
		, m_Bindings(bindings)
	{
		std::vector<VkDescriptorSetLayoutBinding> setLayoutBindings{};
		for (auto& kv : bindings) {
			setLayoutBindings.push_back(kv.second);
		}

		VkDescriptorSetLayoutCreateInfo descriptorSetLayoutInfo{};
		descriptorSetLayoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
		descriptorSetLayoutInfo.bindingCount = static_cast<uint32_t>(setLayoutBindings.size());
		descriptorSetLayoutInfo.pBindings = setLayoutBindings.data();

		if (vkCreateDescriptorSetLayout(
			m_VulkanState.m_Device,
			&descriptorSetLayoutInfo,
			nullptr,
			&m_DescriptorSetLayout) != VK_SUCCESS) {
			throw std::runtime_error("failed to create descriptor set layout!");
		}
	}

	VulkanDescriptorSetLayout::~VulkanDescriptorSetLayout()
	{
		vkDestroyDescriptorSetLayout(m_VulkanState.m_Device, m_DescriptorSetLayout, nullptr);
	}




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

	bool VulkanDescriptorPool::allocateDescriptor(const VkDescriptorSetLayout descriptorSetLayout, VkDescriptorSet& descriptor) const
	{
			VkDescriptorSetAllocateInfo allocInfo{};
			allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
			allocInfo.descriptorPool = m_DescriptorPool;
			allocInfo.pSetLayouts = &descriptorSetLayout;
			allocInfo.descriptorSetCount = 1;

			// Might want to create a "DescriptorPoolManager" class that handles this case, and builds
			// a new pool whenever an old pool fills up. But this is beyond our current scope
			if (vkAllocateDescriptorSets(m_VulkanState.m_Device, &allocInfo, &descriptor) != VK_SUCCESS) {
				return false;
			}
			return true;
	}

	void VulkanDescriptorPool::freeDescriptors(std::vector<VkDescriptorSet>& descriptors) const
	{
		vkFreeDescriptorSets(
			m_VulkanState.m_Device,
			m_DescriptorPool,
			static_cast<uint32_t>(descriptors.size()),
			descriptors.data());
	}

	void VulkanDescriptorPool::resetPool()
	{
		vkResetDescriptorPool(m_VulkanState.m_Device, m_DescriptorPool, 0);
	}





	VulkanDescriptorWriter::VulkanDescriptorWriter(VulkanDescriptorSetLayout& setLayout, VulkanDescriptorPool& pool, const std::vector<VkWriteDescriptorSet>& writters)
		: m_SetLayout(setLayout), m_Pool(pool)
	{
		M_Writes = writters;
	}

	VulkanDescriptorWriter& VulkanDescriptorWriter::WriteBuffer(uint32_t binding, VkDescriptorBufferInfo* bufferInfo)
	{
		assert(m_SetLayout.m_Bindings.count(binding) == 1 && "Layout does not contain specified binding");

		auto& bindingDescription = m_SetLayout.m_Bindings[binding];

		assert(
			bindingDescription.descriptorCount == 1 &&
			"Binding single descriptor info, but binding expects multiple");

		VkWriteDescriptorSet write{};
		write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		write.descriptorType = bindingDescription.descriptorType;
		write.dstBinding = binding;
		write.pBufferInfo = bufferInfo;
		write.descriptorCount = 1;

		M_Writes.push_back(write);
		return *this;
	}

	VulkanDescriptorWriter& VulkanDescriptorWriter::WriteImage(uint32_t binding, VkDescriptorImageInfo* imageInfo)
	{
		assert(m_SetLayout.m_Bindings.count(binding) == 1 && "Layout does not contain specified binding");

		auto& bindingDescription = m_SetLayout.m_Bindings[binding];

		assert(
			bindingDescription.descriptorCount == 1 &&
			"Binding single descriptor info, but binding expects multiple");

		VkWriteDescriptorSet write{};
		write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		write.descriptorType = bindingDescription.descriptorType;
		write.dstBinding = binding;
		write.pImageInfo = imageInfo;
		write.descriptorCount = 1;

		M_Writes.push_back(write);
		return *this;
	}

	bool VulkanDescriptorWriter::Build(VkDescriptorSet& set)
	{
		bool success = m_Pool.allocateDescriptor(m_SetLayout.GetDescriptorSetLayout(), set);
		if (!success) {
			return false;
		}
		OverWrite(set);
		return true;
	}

	void VulkanDescriptorWriter::OverWrite(VkDescriptorSet& set)
	{
		for (auto& write : M_Writes) {
			write.dstSet = set;
		}
		vkUpdateDescriptorSets(m_Pool.m_VulkanState.m_Device, M_Writes.size(), M_Writes.data(), 0, nullptr);
	}

}