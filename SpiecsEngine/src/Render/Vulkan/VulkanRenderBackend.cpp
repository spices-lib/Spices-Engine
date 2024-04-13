#include "Pchheader.h"
#include "VulkanRenderBackend.h"

#include "Render/Renderer/MeshRenderer.h"
#include "Render/Renderer/SkyBoxRenderer.h"

namespace Spiecs {

	VulkanState VulkanRenderBackend::m_VulkanState;
	std::shared_ptr<VulkanDescriptorPool> VulkanRenderBackend::m_VulkanDescriptorPool;

	VulkanRenderBackend::VulkanRenderBackend()
	{
		m_VulkanWindows = std::make_unique<VulkanWindows>(m_VulkanState, 800, 600, "Spiecs Engine");
		m_VulkanInstance = std::make_unique<VulkanInstance>(m_VulkanState, "app", "engine");
		m_VulkanDevice = std::make_shared<VulkanDevice>(m_VulkanState);
		m_VulkanCommandPool = std::make_unique<VulkanCommandPool>(m_VulkanState, m_VulkanDevice->GetQueueHelper().graphicqueuefamily.value());
		m_VulkanCommandBuffer = std::make_unique<VulkanCommandBuffer>(m_VulkanState);
		m_VulkanRenderPass = std::make_unique<VulkanRenderPass>(m_VulkanState, m_VulkanDevice);
		m_VulkanSwapChain = std::make_unique<VulkanSwapChain>(m_VulkanState, m_VulkanDevice);

		m_VulkanDescriptorPool = VulkanDescriptorPool::Builder()
			.SetMaxSets(2000)
			.AddPoolSize(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1000)
			.AddPoolSize(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1000)
			.Build(m_VulkanState);

		// TODO: Move to  
		RendererManager::Get()
			.Push<MeshRenderer>("MeshRenderer", m_VulkanState, m_VulkanDescriptorPool)
			.Push<SkyBoxRenderer>("SkyBoxRenderer", m_VulkanState, m_VulkanDescriptorPool);
	}

	VulkanRenderBackend::~VulkanRenderBackend()
	{
		m_VulkanDescriptorPool = nullptr;

		RendererManager::Get()
			.Pop("MeshRenderer")
			.Pop("SkyBoxRenderer");
	}

	void VulkanRenderBackend::RecreateSwapChain() {
		int width = 0, height = 0;
		glfwGetFramebufferSize(m_VulkanState.m_Windows, &width, &height);
		while (width == 0 || height == 0) {
			glfwGetFramebufferSize(m_VulkanState.m_Windows, &width, &height);
			glfwWaitEvents();
		}
		std::cout << width << "  " << height << std::endl;
		vkDeviceWaitIdle(m_VulkanState.m_Device);
		m_VulkanDevice->RequerySwapChainSupport();

		m_VulkanSwapChain->Destroy();
		m_VulkanSwapChain->Create();
	}

	void VulkanRenderBackend::beginFrame(FrameInfo& frameInfo)
	{
		vkWaitForFences(m_VulkanState.m_Device, 1, &m_VulkanState.m_Fence[frameInfo.m_FrameIndex], VK_TRUE, UINT64_MAX);
		vkResetFences(m_VulkanState.m_Device, 1, &m_VulkanState.m_Fence[frameInfo.m_FrameIndex]);

		VkResult result = vkAcquireNextImageKHR(m_VulkanState.m_Device, m_VulkanState.m_SwapChain, UINT64_MAX, m_VulkanState.m_ImageSemaphore[frameInfo.m_FrameIndex], VK_NULL_HANDLE, &frameInfo.m_Imageindex);
		if (result == VK_ERROR_OUT_OF_DATE_KHR) {
			RecreateSwapChain();
		}
		else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
			throw std::runtime_error("failed to acquire swap chain image!");
		}

		VkCommandBufferBeginInfo beginInfo{};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
		beginInfo.pInheritanceInfo = nullptr;

		VK_CHECK(vkBeginCommandBuffer(m_VulkanState.m_CommandBuffer[frameInfo.m_FrameIndex], &beginInfo));

		VkRenderPassBeginInfo renderPassInfo{};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		renderPassInfo.renderPass = m_VulkanState.m_RenderPass;
		renderPassInfo.framebuffer = m_VulkanSwapChain->GetFramebuffer(frameInfo.m_Imageindex);
		renderPassInfo.renderArea.offset = { 0, 0 };
		renderPassInfo.renderArea.extent = m_VulkanDevice->GetSwapChainSupport().extent;

		std::array<VkClearValue, 2> clearValues = {};
		clearValues[0].color = { {0.1f, 0.1f, 0.1f, 1.0f} };
		clearValues[1].depthStencil = { 1.0f, 0 };

		renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
		renderPassInfo.pClearValues = clearValues.data();

		vkCmdBeginRenderPass(m_VulkanState.m_CommandBuffer[frameInfo.m_FrameIndex], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

		VkViewport viewport{};
		viewport.x = 0.0f;
		viewport.y = 0.0f;
		viewport.width = static_cast<float>(m_VulkanDevice->GetSwapChainSupport().extent.width);
		viewport.height = static_cast<float>(m_VulkanDevice->GetSwapChainSupport().extent.height);
		viewport.minDepth = 0.0f;
		viewport.maxDepth = 1.0f;
		vkCmdSetViewport(m_VulkanState.m_CommandBuffer[frameInfo.m_FrameIndex], 0, 1, &viewport);

		VkRect2D scissor{};
		scissor.offset = { 0, 0 };
		scissor.extent = m_VulkanDevice->GetSwapChainSupport().extent;
		vkCmdSetScissor(m_VulkanState.m_CommandBuffer[frameInfo.m_FrameIndex], 0, 1, &scissor);
	}

	void VulkanRenderBackend::endFrame(FrameInfo& frameInfo)
	{
		vkCmdEndRenderPass(m_VulkanState.m_CommandBuffer[frameInfo.m_FrameIndex]);

		VK_CHECK(vkEndCommandBuffer(m_VulkanState.m_CommandBuffer[frameInfo.m_FrameIndex]));

		vkResetFences(m_VulkanState.m_Device, 1, &m_VulkanState.m_Fence[frameInfo.m_FrameIndex]);

		VkSubmitInfo submitInfo{};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

		VkSemaphore waitSemphores[] = { m_VulkanState.m_ImageSemaphore[frameInfo.m_FrameIndex]};
		VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };

		submitInfo.waitSemaphoreCount = 1;
		submitInfo.pWaitSemaphores = waitSemphores;
		submitInfo.pWaitDstStageMask = waitStages;
		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &m_VulkanState.m_CommandBuffer[frameInfo.m_FrameIndex];

		VkSemaphore signalSemaphores[] = { m_VulkanState.m_QueueSemaphore[frameInfo.m_FrameIndex] };
		submitInfo.signalSemaphoreCount = 1;
		submitInfo.pSignalSemaphores = signalSemaphores;

		VK_CHECK(vkQueueSubmit(m_VulkanState.m_GraphicQueue, 1, &submitInfo, m_VulkanState.m_Fence[frameInfo.m_FrameIndex]));

		VkPresentInfoKHR presentInfo{};
		presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
		presentInfo.waitSemaphoreCount = 1;
		presentInfo.pWaitSemaphores = signalSemaphores;

		VkSwapchainKHR swapChains[] = { m_VulkanState.m_SwapChain };
		presentInfo.swapchainCount = 1;
		presentInfo.pSwapchains = swapChains;
		presentInfo.pImageIndices = &frameInfo.m_Imageindex;

		presentInfo.pResults = nullptr;

		VkResult result = vkQueuePresentKHR(m_VulkanState.m_PresentQueue, &presentInfo);
		if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || m_VulkanWindows->IsResized()) {
			m_VulkanWindows->SetResized(false);
			RecreateSwapChain();
		}
		else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
			throw std::runtime_error::runtime_error("failed to present swap chain image!");
		}

	}

	void VulkanRenderBackend::DrawTest(FrameInfo& frameInfo)
	{
		RendererManager::Run(frameInfo);
	}
}