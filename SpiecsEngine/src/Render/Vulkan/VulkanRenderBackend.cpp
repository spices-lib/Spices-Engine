#include "Pchheader.h"
#include "VulkanRenderBackend.h"
#include "Render/RendererResource/RendererResourcePool.h"
#include "Core/Event/WindowEvent.h"
#include "Systems/SlateSystem.h"

#include "Render/Renderer/SpecificRenderer/PreRenderer.h"
#include "Render/Renderer/SpecificRenderer/BasePassRenderer.h"
#include "Render/Renderer/SpecificRenderer/SlateRenderer.h"
#include "Render/Renderer/SpecificRenderer/SceneComposeRenderer.h"
#include "Render/Renderer/SpecificRenderer/WorldPickRenderer.h"
#include "Render/Renderer/SpecificRenderer/SpriteRenderer.h"
#include "Render/Renderer/SpecificRenderer/WorldPickStage2Renderer.h"

namespace Spiecs {

	const WindowInfo initInfo{ 1920, 1080, "Spiecs Engine"};

	VulkanState VulkanRenderBackend::m_VulkanState;
	std::shared_ptr<VulkanDescriptorPool> VulkanRenderBackend::m_VulkanDescriptorPool;
	std::shared_ptr<RendererResourcePool> VulkanRenderBackend::m_RendererResourcePool;

	VulkanRenderBackend::VulkanRenderBackend()
	{
		m_VulkanWindows       = std::make_unique<VulkanWindows>(m_VulkanState, initInfo);
		m_VulkanInstance      = std::make_unique<VulkanInstance>(m_VulkanState, "app", "engine");
		m_VulkanDevice        = std::make_shared<VulkanDevice>(m_VulkanState);
		m_VulkanCommandPool   = std::make_unique<VulkanCommandPool>(m_VulkanState);
		m_VulkanCommandBuffer = std::make_unique<VulkanCommandBuffer>(m_VulkanState);
		m_VulkanSwapChain     = std::make_unique<VulkanSwapChain>(m_VulkanState, m_VulkanDevice);

		m_VulkanDescriptorPool = VulkanDescriptorPool::Builder()
		.SetPoolFlags(VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT)
		.AddPoolSize(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1000)
		.AddPoolSize(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1000)
		.AddPoolSize(VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1000)
		.AddPoolSize(VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, 1000)
		.Build(m_VulkanState);

		// TODO: Move to  
		m_RendererResourcePool = std::make_shared<RendererResourcePool>();

		RendererManager::Get()
			.Push<PreRenderer>(m_VulkanState, m_VulkanDescriptorPool, m_VulkanDevice, m_RendererResourcePool)
			.Push<BasePassRenderer>(m_VulkanState, m_VulkanDescriptorPool, m_VulkanDevice, m_RendererResourcePool)
			.Push<SceneComposeRenderer>(m_VulkanState, m_VulkanDescriptorPool, m_VulkanDevice, m_RendererResourcePool)
			.Push<SpriteRenderer>(m_VulkanState, m_VulkanDescriptorPool, m_VulkanDevice, m_RendererResourcePool)
			.Push<WorldPickRenderer>(m_VulkanState, m_VulkanDescriptorPool, m_VulkanDevice, m_RendererResourcePool)
			.Push<WorldPickStage2Renderer>(m_VulkanState, m_VulkanDescriptorPool, m_VulkanDevice, m_RendererResourcePool)
			.Push<SlateRenderer>(m_VulkanState, m_VulkanDescriptorPool, m_VulkanDevice, m_RendererResourcePool);
	}

	VulkanRenderBackend::~VulkanRenderBackend()
	{
		m_RendererResourcePool = nullptr;

		m_VulkanDescriptorPool = nullptr;

		RendererManager::Get()
			.Pop("SlateRenderer")
			.Pop("WorldPickStage2Renderer")
			.Pop("WorldPickRenderer")
			.Pop("SpriteRenderer")
			.Pop("SceneComposeRenderer")
			.Pop("BasePassRenderer")
			.Pop("PreRenderer");
	}

	void VulkanRenderBackend::RecreateSwapChain() {
		int width = 0, height = 0;
		glfwGetFramebufferSize(m_VulkanState.m_Windows, &width, &height);
		while (width == 0 || height == 0) 
		{
			glfwGetFramebufferSize(m_VulkanState.m_Windows, &width, &height);
			glfwWaitEvents();
		}
		vkDeviceWaitIdle(m_VulkanState.m_Device);

		/**
		* @brief Create an specific event.
		*/
		WindowResizeOverEvent event(width, height);

		/**
		* @brief Execute the global event function pointer by passing the specific event.
		*/
		Event::GetEventCallbackFn()(event);
	}

	void VulkanRenderBackend::BeginFrame(FrameInfo& frameInfo)
	{
		vkWaitForFences(
			m_VulkanState.m_Device, 
			1, 
			&m_VulkanState.m_Fence[frameInfo.m_FrameIndex], 
			VK_TRUE, UINT64_MAX
		);
		vkResetFences(
			m_VulkanState.m_Device, 
			1, 
			&m_VulkanState.m_Fence[frameInfo.m_FrameIndex]
		);

		VkResult result = vkAcquireNextImageKHR(
			m_VulkanState.m_Device, 
			m_VulkanState.m_SwapChain, 
			UINT64_MAX, 
			m_VulkanState.m_ImageSemaphore[frameInfo.m_FrameIndex], 
			VK_NULL_HANDLE, 
			&frameInfo.m_Imageindex
		);

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

		/**
		* @brief Use Negative Viewport height filp here to handle axis difference.
		* Remember enable device extension (VK_KHR_MAINTENANCE1)
		*/
		VkViewport viewport{};
		viewport.x        = 0.0f;
		viewport.y        =  static_cast<float>(m_VulkanDevice->GetSwapChainSupport().surfaceSize.height);
		viewport.width    =  static_cast<float>(m_VulkanDevice->GetSwapChainSupport().surfaceSize.width);
		viewport.height   = -static_cast<float>(m_VulkanDevice->GetSwapChainSupport().surfaceSize.height);
		viewport.minDepth = 0.0f;
		viewport.maxDepth = 1.0f;

		/**
		* @brief Though we draw world to viewport but not surface,
		* Set Correct viewport here is necessary.
		*/
		if (SlateSystem::GetRegister())
		{
			ImVec2 viewPortPos  = SlateSystem::GetRegister()->GetViewPort()->GetPanelPos();
			ImVec2 viewPortSize = SlateSystem::GetRegister()->GetViewPort()->GetPanelSize();

			viewport.y      =  viewPortSize.y;
			viewport.width  =  viewPortSize.x;
			viewport.height = -viewPortSize.y;
		}

		vkCmdSetViewport(m_VulkanState.m_CommandBuffer[frameInfo.m_FrameIndex], 0, 1, &viewport);

		VkRect2D scissor{};
		scissor.offset = { 0, 0 };
		scissor.extent = m_VulkanDevice->GetSwapChainSupport().surfaceSize;
		vkCmdSetScissor(m_VulkanState.m_CommandBuffer[frameInfo.m_FrameIndex], 0, 1, &scissor);
	}

	void VulkanRenderBackend::EndFrame(FrameInfo& frameInfo)
	{
		VK_CHECK(vkEndCommandBuffer(m_VulkanState.m_CommandBuffer[frameInfo.m_FrameIndex]));

		vkResetFences(
			m_VulkanState.m_Device, 
			1, 
			&m_VulkanState.m_Fence[frameInfo.m_FrameIndex]
		);

		VkSubmitInfo submitInfo{};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

		VkSemaphore waitSemphores[]       = { m_VulkanState.m_ImageSemaphore[frameInfo.m_FrameIndex]};
		VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };

		submitInfo.waitSemaphoreCount   = 1;
		submitInfo.pWaitSemaphores      = waitSemphores;
		submitInfo.pWaitDstStageMask    = waitStages;
		submitInfo.commandBufferCount   = 1;
		submitInfo.pCommandBuffers      = &m_VulkanState.m_CommandBuffer[frameInfo.m_FrameIndex];

		VkSemaphore signalSemaphores[]  = { m_VulkanState.m_QueueSemaphore[frameInfo.m_FrameIndex] };
		submitInfo.signalSemaphoreCount = 1;
		submitInfo.pSignalSemaphores    = signalSemaphores;

		VK_CHECK(vkQueueSubmit(m_VulkanState.m_GraphicQueue, 1, &submitInfo, m_VulkanState.m_Fence[frameInfo.m_FrameIndex]));

		VkPresentInfoKHR presentInfo{};
		presentInfo.sType               = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
		presentInfo.waitSemaphoreCount  = 1;
		presentInfo.pWaitSemaphores     = signalSemaphores;

		VkSwapchainKHR swapChains[]     = { m_VulkanState.m_SwapChain };
		presentInfo.swapchainCount      = 1;
		presentInfo.pSwapchains         = swapChains;
		presentInfo.pImageIndices       = &frameInfo.m_Imageindex;

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

	void VulkanRenderBackend::DrawTest(TimeStep& ts, FrameInfo& frameInfo)
	{
		RendererManager::Run(ts, frameInfo);
	}

	void VulkanRenderBackend::OnEvent(Event& event)
	{
		EventDispatcher dispatcher(event);

		dispatcher.Dispatch<WindowResizeOverEvent>(BIND_EVENT_FN(VulkanRenderBackend::OnWindowResizeOver));
		dispatcher.Dispatch<SlateResizeEvent>(BIND_EVENT_FN(VulkanRenderBackend::OnSlateResize));
	}

	bool VulkanRenderBackend::OnWindowResizeOver(WindowResizeOverEvent& event)
	{
		m_VulkanDevice->RequerySwapChainSupport();

		m_VulkanSwapChain->Destroy();
		m_VulkanSwapChain->Create();

		m_RendererResourcePool->OnSlateResize(event.GetWidth(), event.GetHeight());
		RendererManager::Get().OnWindowResizeOver();

		return false;
	}

	bool VulkanRenderBackend::OnSlateResize(SlateResizeEvent& event)
	{
		m_RendererResourcePool->OnSlateResize(event.GetWidth(), event.GetHeight());
		RendererManager::Get().OnSlateResize();

		return false;
	}
}