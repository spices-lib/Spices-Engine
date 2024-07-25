/**
* @file VulkanDescriptor.h.
* @brief The VulkanDescriptorPool & VulkanDescriptorSetLayout & VulkanDescriptorSet Class Definitions.
* @author Spices.
*/

#pragma once
#include "Core/Core.h"
#include "VulkanUtils.h"

#include <unordered_map>
#include <vector>

namespace Spices {

    /**
    * @brief VulkanDescriptorPool Class.
    * This class is the wrapper of VkDescriptorPool.
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
            Builder() = default;

            /**
            * @brief Destructor Function.
            */
            virtual ~Builder() = default;

            /**
            * @brief Add a size of specific descriptor type.
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
            * @brief The max descriptor set nums that this pool can assign.
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
        * @param[in] maxSets The maximum descriptor set count this pool can assign.
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
        virtual ~VulkanDescriptorPool() override;
        
        /**
        * @brief Reset this pool.
        * @note Not in use.
        */
        void resetPool();

        /**
        * @brief Get the VkDescriptorPool this class wrapped.
        * @return Returns the VkDescriptorPool.
        */
        inline VkDescriptorPool& GetPool() { return m_DescriptorPool; }

    private:

        /**
        * @brief The VkDescriptorPool this class wrapped.
        */
        VkDescriptorPool m_DescriptorPool;
    };

    /**
    * @brief This Class is a Wrapper of VkDescriptorSetLayout.
    * Usually used as a member variable in VulkanDescriptorSet.
    * Single use is also allowed.
    */
    class VulkanDescriptorSetLayout : public VulkanObject
    {
    public:

        /**
        * @brief Constructor Function.
        * @param[in] vulkanState The core vulkan objects that in use.
        */
        VulkanDescriptorSetLayout(
            VulkanState& vulkanState
        ) 
            : VulkanObject(vulkanState) 
        {}

        /**
        * @brief Destructor Function.
        */
        virtual ~VulkanDescriptorSetLayout() override;

        /**
        * @brief Build a VkDescriptorSetLayout with bindings.
        * @param[in] bindings all the bindings used in a VkDescriptorSetLayout.
        * @param[in] caption DescriptorSetLayout Name.
        */
        void BuildDescriptorSetLayout(
            const std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding>& bindings,
            const std::string&                                                caption = ""
        );

        /**
        * @brief Build a bindless texture VkDescriptorSetLayout with bindings.
        * @param[in] bindings all the bindings used in a VkDescriptorSetLayout.
        * @param[in] caption DescriptorSetLayout Name.
        */
        void BuildBindLessTextureDescriptorSetLayout(
            const std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding>& bindings,
            const std::string&                                                caption = ""
        );

        /**
        * @brief Get a VkDescriptorSetLayout.
        * @return Returns the VkDescriptorSetLayout.
        */
        inline VkDescriptorSetLayout& Get() { return m_Layout; }
        
    private:

        /**
        * @brief The VkDescriptorSetLayout this class handled.
        */
        VkDescriptorSetLayout m_Layout{};
    };
    
    /**
    * @brief This Class is a wrapper of VkDescriptorSet.
    */
    class VulkanDescriptorSet : public VulkanObject
    {
    public:

        /**
        * @brief Simply definitions.
        */
        using ImageInfo  = std::unordered_map<uint32_t, std::vector<VkDescriptorImageInfo>>;
        using BufferInfo = std::unordered_map<uint32_t, VkDescriptorBufferInfo>;
        
    public:

        /**
        * @brief Constructor Function.
        * @param[in] vulkanState The core vulkan objects that in use.
        * @param[in] pool VulkanDescriptorPool.
        */
        VulkanDescriptorSet(
            VulkanState&                          vulkanState , 
            std::shared_ptr<VulkanDescriptorPool> pool
        )
            : VulkanObject(vulkanState)
            , m_Layout(vulkanState)
            , m_Pool(pool)
        {}
        
        /**
        * @brief Destructor Function.
        */
        virtual ~VulkanDescriptorSet() override;

        /**
        * @brief Add a binding to this descriptor set.
        * @param[in] binding Which binging location is.
        * @param[in] descriptorType What type it is.
        * @param[in] stageFlags Shader access flags.
        * @param[in] count Array num.
        */
        void AddBinding(
            uint32_t            binding,
            VkDescriptorType    descriptorType,
            VkShaderStageFlags  stageFlags,
            uint32_t            count = 1
        );

        /**
        * @brief Build this.
        * @param[in] createrName The object this descriptor set belongs to.
        */
        void BuildDescriptorSet(const std::string& createrName);
        
        /**
        * @brief Build this bindless.
        * @param[in] createrName The object this descriptor set belongs to.
        */
        void BuildBindLessTextureDescriptorSet(const std::string& createrName);

        /**
        * @brief Update this descriptor set.
        * @param[in] imageInfo ImageInfo.
        * @param[in] bufferInfo BufferInfo.
        * @param[in] accel VkAccelerationStructureKHR.
        */
        void UpdateDescriptorSet(
            ImageInfo&                        imageInfo  ,
            BufferInfo&                       bufferInfo ,
            const VkAccelerationStructureKHR& accel = VK_NULL_HANDLE
        ) const;

        /**
        * @brief Update this descriptor set.
        * @param[in] bufferInfo BufferInfo.
        */
        void UpdateDescriptorSet(
            BufferInfo& bufferInfo
        ) const;

        /**
        * @brief Update this descriptor set.
        * @param[in] imageInfo ImageInfo.
        */
        void UpdateDescriptorSet(
            ImageInfo&  imageInfo
        ) const;
        
        /**
        * @brief Update this bindless descriptor set.
        * @param[in] imageInfo ImageInfo.
        */
        void UpdateBindLessTextureDescriptorSet(
            ImageInfo& imageInfo
        ) const;

        /**
        * @brief Get a VkDescriptorSet.
        * @return Returns the VkDescriptorSet.
        */
        VkDescriptorSet& Get() { return m_DescriptorSet; }

        /**
        * @brief Get a VkDescriptorSetLayout.
        * @return Returns the VkDescriptorSetLayout.
        */
        VkDescriptorSetLayout& GetRowSetLayout() { return m_Layout.Get(); }
        
    private:

        /**
        * @brief The bindings this descriptor set in use.
        */
        std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> m_Bindings;

        /**
        * @brief The VkDescriptorSet this class handled.
        */
        VkDescriptorSet m_DescriptorSet{};

        /**
        * @brief The VulkanDescriptorSetLayout this descriptor set in use.
        */
        VulkanDescriptorSetLayout m_Layout;

        /**
        * @brief The VulkanDescriptorPool this descriptor set in use.
        */
        std::shared_ptr<VulkanDescriptorPool> m_Pool;
    };
}