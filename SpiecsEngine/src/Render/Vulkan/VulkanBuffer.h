/**
* @file VulkanBuffer.h.
* @brief The VulkanBuffer Class Definitions.
* @author Spiecs.
*/

#pragma once
#include "Core/Core.h"
#include "VulkanUtils.h"

namespace Spiecs {

	/**
	* @brief This Class is a Wapper of VulkanBuffer.
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
		{};

		/**
		* @brief Constructor Function.
		* Create VkBuffer.
		* @param[in] vulkanState The global VulkanState.
		* @param[in] size The buffer size(bytes).
		* @param[in] usage The buffer usage.
		* @param[in] properties The buffer memory used flags.
		*/
		VulkanBuffer(
			VulkanState&          vulkanState  , 
			VkDeviceSize          size         , 
			VkBufferUsageFlags    usage        , 
			VkMemoryPropertyFlags properties
		);

		/**
		* @brief Destructor Function.
		*/
		virtual ~VulkanBuffer();

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
		* @brief Get VkDeviceMemory.
		* @return Returns the VkDeviceMemory.
		*/
		inline VkDeviceMemory& GetMomory() { return m_BufferMemory; };

		/**
		* @brief Get VkBuffer Address.
		* @return Returns the VkDeviceAddress.
		*/
		VkDeviceAddress& GetAddress();

		/**
		* @brief Get VkBuffer.
		* @return Returns the VkBuffer.
		*/
		inline VkBuffer& Get() { return m_Buffer; };

		/**
		* @brief Map buffer video memory to a local memory.
		* @param[in] size The buffer size.
		* @param[in] offset The buffer video memory offest.
		*/
		void Map(
			VkDeviceSize size   = VK_WHOLE_SIZE , 
			VkDeviceSize offset = 0
		);

		/**
		* @brief Get VkDescriptorBufferInfo.
		* @param[in] size The buffer size.
		* @param[in] offset The buffer video memory offest.
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
		* @param[in] offset The buffer video memory offest.
		*/
		void WriteToBuffer(
			void*        data, 
			VkDeviceSize size   = VK_WHOLE_SIZE , 
			VkDeviceSize offset = 0
		);

		/**
		* @brief Flush the buffer's viedo memory data.
		* @param[in] size The buffer size.
		* @param[in] offset The buffer video memory offest.
		*/
		void Flush(
			VkDeviceSize size   = VK_WHOLE_SIZE, 
			VkDeviceSize offset = 0
		);

	private:

		/**
		* @brief Create a buffer.
		* @param[in] vulkanState The global VulkanState.
		* @param[in] size The buffer size.
		* @param[in] usage The buffer usage.
		* @param[in] properties The buffer memory requirement flags.
		*/
		void CreateBuffer(
			VulkanState&          vulkanState , 
			VkDeviceSize          size        , 
			VkBufferUsageFlags    usage       , 
			VkMemoryPropertyFlags properties
		);

	private:

		/**
		* @brief The buffer size.
		*/
		VkDeviceSize m_DeviceSize{};

		/**
		* @brief The buffer usage.
		*/
		VkBufferUsageFlags m_Uasge{};

		/**
		* @brief The buffer memory requirement flags.
		*/
		VkMemoryPropertyFlags m_Flags{};

		/**
		* @brief The buffer this class handled.
		*/
		VkBuffer m_Buffer{};

		/**
		* @brief The buffer video memory.
		*/
		VkDeviceMemory m_BufferMemory{};

		/**
		* @brief The buffer gpu address.
		*/
		VkDeviceAddress m_BufferAddress{};

		/**
		* @brief The mapped buffer local memory.
		*/
		void* m_LocalMemory = nullptr;

		/**
		* @brief VkDescriptorBufferInfo.
		*/
		VkDescriptorBufferInfo m_BufferInfo{};
	};
}