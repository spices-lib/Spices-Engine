/**
* @file Attribute.h.
* @brief The Attribute Class Definitions and Implementation.
* @author Spices.
*/

#pragma once
#include "Core/Core.h"
#include "Render/Vulkan/VulkanAccelerationStructure.h"

namespace Spices {

	/**
	* @brief AccelStructure Wrapper.
	*/
	struct AccelKHR
	{
		std::shared_ptr<VulkanAccelerationStructure> accel = nullptr;
		std::shared_ptr<VulkanBuffer> buffer = nullptr;
	};

	/**
	* @brief MeshResource's item template.
	* @tparam T specific item value type.
	*/
	template<typename T>
	struct Attribute
	{
		/**
		* @brief Constructor Function.
		*/
		Attribute();

		/**
		* @brief Destructor Function.
		*/
		virtual ~Attribute();

		/**
		* @brief Create Attribute Buffer.
		* @param[in] name Buffer Debug Name.
		* @param[in] usage Buffer Usage.
		*/
		void CreateBuffer(const std::string& name, VkBufferUsageFlags usage = 0);

		/**
		* @brief Attribute Data Array.
		*/
		std::shared_ptr<std::vector<T>> attributes;

		/**
		* @brief Attribute Buffer.
		*/
		std::shared_ptr<VulkanBuffer> buffer;
	};

	template<typename T>
	inline Attribute<T>::Attribute()
		: attributes(nullptr)
		, buffer(nullptr)
	{
		SPICES_PROFILE_ZONE;

		if (!attributes)
		{
			attributes = std::make_shared<std::vector<T>>();
		}
	}

	template<typename T>
	inline Attribute<T>::~Attribute()
	{
		SPICES_PROFILE_ZONE;

		attributes = nullptr;
		buffer = nullptr;
	}

	template<typename T>
	inline void Attribute<T>::CreateBuffer(const std::string& name, VkBufferUsageFlags usage)
	{
		SPICES_PROFILE_ZONE;

		VkDeviceSize bufferSize = sizeof((*attributes)[0]) * attributes->size();

		if (bufferSize > 0)
		{
			VulkanBuffer stagingBuffer(
				VulkanRenderBackend::GetState()      ,
				"StagingBuffer"                      ,
				bufferSize                           ,
				VK_BUFFER_USAGE_TRANSFER_SRC_BIT     ,
				VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT  |
				VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
			);

			buffer = std::make_shared<VulkanBuffer>(
				VulkanRenderBackend::GetState()           ,
				name                                      ,
				bufferSize                                ,
				VK_BUFFER_USAGE_TRANSFER_DST_BIT          |
				VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT |
				usage                                     ,
				VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
			);

			stagingBuffer.WriteToBuffer(attributes->data());
			buffer->CopyBuffer(stagingBuffer.Get(), buffer->Get(), bufferSize);
		}
		else
		{
			bufferSize = 16;

			VulkanBuffer stagingBuffer(
				VulkanRenderBackend::GetState()      ,
				"StagingBuffer"                      ,
				bufferSize                           ,
				VK_BUFFER_USAGE_TRANSFER_SRC_BIT     ,
				VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT  |
				VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
			);

			buffer = std::make_shared<VulkanBuffer>(
				VulkanRenderBackend::GetState()           ,
				name                                      ,
				bufferSize                                ,
				VK_BUFFER_USAGE_TRANSFER_DST_BIT          |
				VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT |
				usage                                     ,
				VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
			);

			glm::vec4 empty = glm::vec4(0.0f);
			stagingBuffer.WriteToBuffer(&empty);
			buffer->CopyBuffer(stagingBuffer.Get(), buffer->Get(), bufferSize);
		}
	}
}