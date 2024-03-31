#pragma once
#include "Core/Core.h"
#include "VulkanUtils.h"
#include "VulkanDevice.h"

namespace Spiecs {

	class VulkanRenderPass : public VulkanObject
	{
	public:
		VulkanRenderPass(VulkanState& vulkanState, std::shared_ptr<VulkanDevice> vulkanDevice);
		virtual ~VulkanRenderPass();

	private:
		std::shared_ptr<VulkanDevice> m_VulkanDevice;
	};
}