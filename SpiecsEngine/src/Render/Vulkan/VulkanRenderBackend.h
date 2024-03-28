#pragma once
#include "Core/Core.h"
#include "VulkanUtils.h"

#include "VulkanWindows.h"
#include "VulkanInstance.h"
#include "VulkanDevice.h"
#include "VulkanCommandBuffer.h"

namespace Spiecs {

	class VulkanRenderBackend
	{
	public:
		VulkanRenderBackend();
		virtual ~VulkanRenderBackend();

		VulkanRenderBackend(const VulkanRenderBackend&) = delete;
		VulkanRenderBackend& operator=(const VulkanRenderBackend&) = delete;

	private:
		static VulkanState m_VulkanState;

		std::unique_ptr<VulkanWindows> m_VulkanWindows;
		std::unique_ptr<VulkanInstance> m_VulkanInstance;
		std::unique_ptr<VulkanDevice> m_VulkanDevice;
		std::unique_ptr<VulkanCommandPool> m_VulkanCommandPool;
		std::unique_ptr<VulkanCommandBuffer> m_VulkanCommandBuffer;
	};
}