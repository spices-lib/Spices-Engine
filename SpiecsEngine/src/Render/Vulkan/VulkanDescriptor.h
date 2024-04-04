#pragma once
#include "Core/Core.h"
#include "VulkanUtils.h"

#include <unordered_map>
#include <vector>

namespace Spiecs {

	class VulkanDescriptorSetLayout : public VulkanObject
	{
	public:
        class Builder 
        {
        public:
            Builder() {};
            Builder(VulkanDescriptorSetLayout* oldLayout);

            Builder& AddBinding(
                uint32_t binding,
                VkDescriptorType descriptorType,
                VkShaderStageFlags stageFlags,
                uint32_t count = 1
            );

            std::unique_ptr<VulkanDescriptorSetLayout> Build(VulkanState& vulkanState) const;

        private:
            std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> m_Bindings{};
        };

    public:
		VulkanDescriptorSetLayout(VulkanState& vulkanState, std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> bindings);
		virtual ~VulkanDescriptorSetLayout();

        VkDescriptorSetLayout& GetDescriptorSetLayout() { return m_DescriptorSetLayout; };

	private:
        VkDescriptorSetLayout m_DescriptorSetLayout;
        std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> m_Bindings{};

        friend class VulkanDescriptorWriter;
	};

    class VulkanDescriptorPool : public VulkanObject
    {
    public:
        class Builder {
        public:
            Builder() {};

            Builder& AddPoolSize(VkDescriptorType descriptorType, uint32_t count);
            Builder& SetPoolFlags(VkDescriptorPoolCreateFlags flags);
            Builder& SetMaxSets(uint32_t count);
            std::shared_ptr<VulkanDescriptorPool> Build(VulkanState& vulkanState) const;

        private:
            std::vector<VkDescriptorPoolSize> m_PoolSizes{};
            uint32_t m_MaxSets = 1000;
            VkDescriptorPoolCreateFlags m_PoolFlags = 0;
        };

    public:
        VulkanDescriptorPool(VulkanState& vulkanState, uint32_t maxSets, VkDescriptorPoolCreateFlags poolFlags, const std::vector<VkDescriptorPoolSize>& poolSizes);
        virtual ~VulkanDescriptorPool();

        bool allocateDescriptor(const VkDescriptorSetLayout descriptorSetLayout, VkDescriptorSet& descriptor) const;
        void freeDescriptors(std::vector<VkDescriptorSet>& descriptors) const;
        void resetPool();

    private:
        VkDescriptorPool m_DescriptorPool;

        friend class VulkanDescriptorWriter;
    };

    class VulkanDescriptorWriter
    {
    public:
        VulkanDescriptorWriter(VulkanDescriptorSetLayout& setLayout, VulkanDescriptorPool& pool)
            : m_SetLayout(setLayout), m_Pool(pool) {};

        VulkanDescriptorWriter& WriteBuffer(uint32_t binding, VkDescriptorBufferInfo* bufferInfo);
        VulkanDescriptorWriter& WriteImage(uint32_t binding, VkDescriptorImageInfo* imageInfo);

        bool Build(VkDescriptorSet& set);
        void OverWrite(VkDescriptorSet& set);

    private:
        VulkanDescriptorSetLayout& m_SetLayout;
        VulkanDescriptorPool& m_Pool;
        std::vector<VkWriteDescriptorSet> M_Writes;

    };
}