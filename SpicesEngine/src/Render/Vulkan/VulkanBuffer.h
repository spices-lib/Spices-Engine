/**
* @file VulkanBuffer.h.
* @brief The VulkanBuffer Class Definitions.
* @author Spices.
*/

#pragma once
#include "Core/Core.h"
#include "VulkanUtils.h"

namespace Spices {

	/**
	* @brief This Class is a Wrapper of VulkanBuffer.
	*/
	class VulkanBuffer : public VulkanObject
	{
	public:

		/**
		* @brief Constructor Function.
		* Create VkBuffer.
		* @param[in] vulkanState The global VulkanState.
		*/
		VulkanBuffer(
			VulkanState& vulkanState
		) 
			: VulkanObject(vulkanState)
		{}

		/**
		* @brief Constructor Function.
		* Create VkBuffer.
		* @param[in] vulkanState The global VulkanState.
		* @param[in] name Buffer Name.
		* @param[in] size The buffer size(bytes).
		* @param[in] usage The buffer usage.
		* @param[in] properties The buffer memory used flags.
		*/
		VulkanBuffer(
			VulkanState&          vulkanState  ,
			const std::string&    name         ,
			VkDeviceSize          size         , 
			VkBufferUsageFlags    usage        , 
			VkMemoryPropertyFlags properties
		);

		/**
		* @brief Destructor Function.
		*/
		virtual ~VulkanBuffer() override;

		/**
		* @brief Copy data from a buffer to another.
		* @param[in] srcBuffer The buffer copy from.
		* @param[in] dstBuffer The buffer copy to.
		* @param[in] size The buffer size(byres).
		*/
		void CopyBuffer(
			VkBuffer srcBuffer  , 
			VkBuffer dstBuffer  , 
			VkDeviceSize size
		);

		/**
		* @brief Get VkBuffer Address.
		* @return Returns the VkDeviceAddress.
		*/
		VkDeviceAddress& GetAddress();

		/**
		* @brief Get VkBuffer.
		* @return Returns the VkBuffer.
		*/
		inline VkBuffer& Get() { return m_Buffer; }

		/**
		* @brief Get VkDescriptorBufferInfo.
		* @param[in] size The buffer size.
		* @param[in] offset The buffer video memory offset.
		* @return Returns the VkDescriptorBufferInfo.
		*/
		VkDescriptorBufferInfo* GetBufferInfo(
			VkDeviceSize size   = VK_WHOLE_SIZE, 
			VkDeviceSize offset = 0
		);

		/**
		* @brief Write data to buffer.
		* @param[in] data The data copy from.
		* @param[in] size The buffer size.
		* @param[in] offset The buffer video memory offset.
		*/
		void WriteToBuffer(
			const void*        data, 
			VkDeviceSize size   = VK_WHOLE_SIZE , 
			VkDeviceSize offset = 0
		);

		/**
		* @brief Write data from buffer.
		* @param[in] data The data copy to.
		* @param[in] size The buffer size.
		* @param[in] offset The buffer video memory offset.
		*/
		void WriteFromBuffer(
			void* data,
			VkDeviceSize size = VK_WHOLE_SIZE,
			VkDeviceSize offset = 0
		);

		/**
		* @brief Flush the buffer's video memory data.
		* @param[in] size The buffer size.
		* @param[in] offset The buffer video memory offset.
		*/
		void Flush(
			VkDeviceSize size   = VK_WHOLE_SIZE, 
			VkDeviceSize offset = 0
		) const;

		/**
		* @brief Get Buffer Size.
		* @return Returns Buffer Size.
		*/
		uint64_t GetSize() const { return m_DeviceSize; }

	private:

		/**
		* @brief Create a buffer.
		* @param[in] vulkanState The global VulkanState.
		* @param[in] name Buffer name
		* @param[in] size The buffer size.
		* @param[in] usage The buffer usage.
		* @param[in] properties The buffer memory requirement flags.
		*/
		void CreateBuffer(
			VulkanState&          vulkanState ,
			const std::string&    name        ,
			VkDeviceSize          size        , 
			VkBufferUsageFlags    usage       , 
			VkMemoryPropertyFlags properties
		);

		/**
		* @brief Map buffer video memory to a local memory.
		* @param[in] size The buffer size.
		* @param[in] offset The buffer video memory offset.
		*/
		void Map(
			VkDeviceSize size   = VK_WHOLE_SIZE , 
			VkDeviceSize offset = 0
		);

	private:

		/**
		* @brief Buffer Name.
		*/
		std::string m_Name;

		/**
		* @brief The buffer size.
		*/
		VkDeviceSize m_DeviceSize{};

		/**
		* @brief The buffer usage.
		*/
		VkBufferUsageFlags m_Usage {};

		/**
		* @brief The buffer memory requirement flags.
		*/
		VkMemoryPropertyFlags m_Flags{};

		/**
		* @brief The buffer this class handled.
		*/
		VkBuffer m_Buffer {};

		/**
		* @brief The buffer gpu address.
		*/
		VkDeviceAddress m_BufferAddress{};

#ifdef VMA_ALLOCATOR

		/**
		* @brief VMA allocation.
		*/
		VmaAllocation m_Alloc{};

#else

		/**
		* @brief The buffer video memory.
		*/
		VkDeviceMemory m_BufferMemory{};

		/**
		* @brief The mapped buffer local memory.
		*/
		void* m_LocalMemory = nullptr;

#endif

		/**
		* @brief VkDescriptorBufferInfo.
		*/
		VkDescriptorBufferInfo m_BufferInfo{};
	};
}