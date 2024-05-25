/**
* @file VulkanDescriptor.h.
* @brief The VulkanDescriptorPool & VulkanDescriptorSetLayout & VulkanDescriptorSet Class Definitions.
* @author Spiecs.
*/

#pragma once
#include "Core/Core.h"
#include "VulkanUtils.h"

#include <unordered_map>
#include <vector>

namespace Spiecs {

    /**
    * @brief VulkanDescriptorPool Class.
    * This class is the wapper of VkDescriptorPool.
    */
    class VulkanDescriptorPool : public VulkanObject
    {
    public:

        /**
        * @brief VulkanDescriptorPool::Builder Class.
        * This class is defines how to build a VulkanDescriptorPool.
        */
        class Builder 
        {
        public:

            /**
            * @brief Constructor Function.
            */
            Builder() {};

            /**
            * @brief Destructor Function.
            */
            virtual ~Builder() {};

            /**
            * @brief Add a size of specific descriptortype.
            * @param[in] descriptorType The specific type of descriptor.
            * @param[in] count The max nums this type will occupied.
            * @return Returns a reference of this.
            */
            Builder& AddPoolSize(VkDescriptorType descriptorType, uint32_t count);

            /**
            * @brief Add a type identify to VkDescriptorPool.
            * @param[in] flags The specific VkDescriptorPool type identify.
            * @return Returns a reference of this.
            */
            Builder& SetPoolFlags(VkDescriptorPoolCreateFlags flags);

            /**
            * @brief Build a shared pointer of VulkanDescriptorPool.
            * @param[in] vulkanState The core vulkan objects that in use.
            * @return Returns the shared pointer of VulkanDescriptorPool.
            */
            std::shared_ptr<VulkanDescriptorPool> Build(VulkanState& vulkanState) const;

        private:

            /**
            * @brief The vector of specific descriptor type's max count.
            */
            std::vector<VkDescriptorPoolSize> m_PoolSizes{};

            /**
            * @brief The max descriptorset nums that this pool can assign.
            */
            uint32_t m_MaxSets = 1000;

            /**
            * @brief The specific VkDescriptorPool type identify.
            */
            VkDescriptorPoolCreateFlags m_PoolFlags = 0;
        };

    public:

        /**
        * @brief Constructor Function.
        * @param[in] vulkanState The core vulkan objects that in use.
        * @param[in] maxSets The maxumn descriptorset count this pool can assign.
        * @param[in] poolFlags The specific VkDescriptorPool type identify.
        * @param[in] poolSizes The vector of specific descriptor type's max count.
        */
        VulkanDescriptorPool(
            VulkanState&                             vulkanState , 
            uint32_t                                 maxSets     , 
            VkDescriptorPoolCreateFlags              poolFlags   , 
            const std::vector<VkDescriptorPoolSize>& poolSizes
        );

        /**
        * @brief Destructor Function.
        */
        virtual ~VulkanDescriptorPool();
        
        /**
        * @brief Reset this pool.
        * @note Not in use.
        */
        void resetPool();

        /**
        * @brief Get the VkDescriptorPool this class wappered.
        * @return Returns the VkDescriptorPool.
        */
        inline VkDescriptorPool& GetPool() { return m_DescriptorPool; };

    private:

        /**
        * @brief The VkDescriptorPool this class wappered.
        */
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
        
        void UpdateDescriptorSet(ImageInfo&  imageInfo, BufferInfo& bufferInfo);

        void UpdateDescriptorSet(BufferInfo& bufferInfo);

        void UpdateDescriptorSet(ImageInfo& imageInfo);
        
        VkDescriptorSet& Get() { return m_DescriptorSet; };
        VkDescriptorSetLayout& GetRowSetLayout() { return m_Layout.Get(); };
        
    private:
        std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> m_Bindings;
        VkDescriptorSet m_DescriptorSet;

        VulkanDescriptorSetLayout m_Layout;
        std::shared_ptr<VulkanDescriptorPool> m_Pool;
    };
}