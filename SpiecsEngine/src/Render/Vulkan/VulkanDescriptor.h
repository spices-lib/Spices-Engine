#pragma once
#include "Core/Core.h"
#include "VulkanUtils.h"

#include <unordered_map>
#include <vector>

namespace Spiecs {

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
        
        void resetPool();
        inline VkDescriptorPool& GetPool() { return m_DescriptorPool; };

    private:
        VkDescriptorPool m_DescriptorPool;
    };

    class VulkanDescriptorSetLayout : public VulkanObject
    {
    public:
        VulkanDescriptorSetLayout(VulkanState& vulkanState) : VulkanObject(vulkanState) {};
        virtual ~VulkanDescriptorSetLayout();

        void BuildDescriptorSetLayout(const std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding>& bindings);
        VkDescriptorSetLayout& Get() { return m_Layout; };
        
    private:
        VkDescriptorSetLayout m_Layout;
    };
    
    class VulkanDescriptorSet : public VulkanObject
    {
    public:
        using ImageInfo  = std::unordered_map<uint32_t, std::vector<VkDescriptorImageInfo>>;
        using BufferInfo = std::unordered_map<uint32_t, VkDescriptorBufferInfo>;
        
    public:
        VulkanDescriptorSet(VulkanState& vulkanState, std::shared_ptr<VulkanDescriptorPool> pool)
            : VulkanObject(vulkanState)
            , m_Layout(vulkanState)
            , m_Pool(pool)
        {};
        
        virtual ~VulkanDescriptorSet();

        void AddBinding(
            uint32_t            binding,
            VkDescriptorType    descriptorType,
            VkShaderStageFlags  stageFlags,
            uint32_t            count = 1
        );

        void BuildDescriptorSet();
        
        void UpdateDescriptorSet(
            ImageInfo&  imageInfo,
            BufferInfo& bufferInfo
        );
        
        VkDescriptorSet& Get() { return m_DescriptorSet; };
        
    private:
        std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> m_Bindings;
        VkDescriptorSet m_DescriptorSet;

        VulkanDescriptorSetLayout m_Layout;
        std::shared_ptr<VulkanDescriptorPool> m_Pool;
    };
}