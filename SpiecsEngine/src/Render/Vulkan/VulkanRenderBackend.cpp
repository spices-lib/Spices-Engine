#include "pchheader.h"
#include "VulkanRenderBackend.h"

namespace Spiecs {

	VulkanState VulkanRenderBackend::m_VulkanState;

	VulkanRenderBackend::VulkanRenderBackend()
	{
		m_VulkanWindows = std::make_unique<VulkanWindows>(m_VulkanState, 800, 600, "Spiecs Engine");
		m_VulkanInstance = std::make_unique<VulkanInstance>(m_VulkanState, "app", "engine");
		m_VulkanDevice = std::make_shared<VulkanDevice>(m_VulkanState);
		m_VulkanCommandPool = std::make_unique<VulkanCommandPool>(m_VulkanState, m_VulkanDevice->GetQueueHelper().graphicqueuefamily.value());
		m_VulkanCommandBuffer = std::make_unique<VulkanCommandBuffer>(m_VulkanState);
		m_VulkanRenderPass = std::make_unique<VulkanRenderPass>(m_VulkanState, m_VulkanDevice);
		m_VulkanSwapChain = std::make_unique<VulkanSwapChain>(m_VulkanState, m_VulkanDevice);
	}

	VulkanRenderBackend::~VulkanRenderBackend()
	{

	}
}