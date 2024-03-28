#pragma once
#include "Core/Core.h"
#include "VulkanUtils.h"

#include "VulkanWindows.h"
#include "VulkanInstance.h"
#include "VulkanDevice.h"

namespace Spiecs {

	class VulkanRenderBackend
	{
	public:
		VulkanRenderBackend();
		virtual ~VulkanRenderBackend();

		VulkanRenderBackend(const VulkanRenderBackend&) = delete;
		VulkanRenderBackend& operator=(const VulkanRenderBackend&) = delete;

	private:
		std::shared_ptr<VulkanWindows> m_Windows;
		std::shared_ptr<VulkanInstance> m_Instance;
		std::shared_ptr<VulkanDevice> m_Device;
	};
}