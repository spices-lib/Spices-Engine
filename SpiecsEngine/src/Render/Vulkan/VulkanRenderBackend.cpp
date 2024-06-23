/**
* @file VulkanRenderBackend.cpp.
* @brief The VulkanRenderBackend Class Implementation.
* @author Spiecs.
*/

#include "Pchheader.h"
#include "VulkanRenderBackend.h"

#include "Render/RendererResource/RendererResourcePool.h"
#include "Core/Event/WindowEvent.h"
#include "Systems/SlateSystem.h"

#include "Render/Renderer/SpecificRenderer/PreRenderer.h"
#include "Render/Renderer/SpecificRenderer/BasePassRenderer.h"
#include "Render/Renderer/SpecificRenderer/ShadowRenderer.h"
#include "Render/Renderer/SpecificRenderer/SlateRenderer.h"
#include "Render/Renderer/SpecificRenderer/SceneComposeRenderer.h"
#include "Render/Renderer/SpecificRenderer/WorldPickRenderer.h"
#include "Render/Renderer/SpecificRenderer/SpriteRenderer.h"
#include "Render/Renderer/SpecificRenderer/WorldPickStage2Renderer.h"
#include "Render/Renderer/SpecificRenderer/TestRenderer.h"

namespace Spiecs {

	/**
	* @brief Window create parameter.
	*/
	const WindowInfo initInfo{ 1600, 900, "Spiecs Engine"};

	VulkanState                           VulkanRenderBackend::m_VulkanState;
	std::shared_ptr<VulkanDescriptorPool> VulkanRenderBackend::m_VulkanDescriptorPool;
	std::shared_ptr<RendererResourcePool> VulkanRenderBackend::m_RendererResourcePool;

	VulkanRenderBackend::VulkanRenderBackend()
	{
		SPIECS_PROFILE_ZONE;

		/**
		* @brief Create basic Vulkan Objects.
		*/
		{
			m_VulkanWindows       = std::make_unique<VulkanWindows>      (m_VulkanState, initInfo);
			m_VulkanInstance      = std::make_unique<VulkanInstance>     (m_VulkanState, "app", "engine");

			/**
			* @brief Init Vulkan Debug Functions.
			*/
			{
				VulkanDebugUtils::Init(m_VulkanState.m_Instance);
			}

			m_VulkanDevice        = std::make_shared<VulkanDevice>       (m_VulkanState);
			m_VulkanCommandPool   = std::make_unique<VulkanCommandPool>  (m_VulkanState);
			m_VulkanCommandBuffer = std::make_unique<VulkanCommandBuffer>(m_VulkanState);
			m_VulkanSwapChain     = std::make_unique<VulkanSwapChain>    (m_VulkanState, m_VulkanDevice);
			//m_VulkanRayTracing    = std::make_unique<VulkanRayTracing>   (m_VulkanState);
		}

		/**
		* @brief Create VulkanDescriptorPool.
		*/
		{
			m_VulkanDescriptorPool = VulkanDescriptorPool::Builder()
			.SetPoolFlags(VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT)
			.AddPoolSize(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1000)
			.AddPoolSize(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1000)
			.AddPoolSize(VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1000)
			.AddPoolSize(VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, 1000)
			.Build(m_VulkanState);
		}

		/**
		* @brief Create RendererResourcePool.
		* @todo Move to  
		*/
		{
			m_RendererResourcePool = std::make_shared<RendererResourcePool>();
		}

		/**
		* @brief Create all Specific Renderer.
		*/
		{
			RendererManager::Get()
			.Push<PreRenderer>            (m_VulkanState, m_VulkanDescriptorPool, m_VulkanDevice, m_RendererResourcePool)
			.Push<BasePassRenderer>       (m_VulkanState, m_VulkanDescriptorPool, m_VulkanDevice, m_RendererResourcePool)
			.Push<ShadowRenderer>         (m_VulkanState, m_VulkanDescriptorPool, m_VulkanDevice, m_RendererResourcePool)
			.Push<SceneComposeRenderer>   (m_VulkanState, m_VulkanDescriptorPool, m_VulkanDevice, m_RendererResourcePool)
			.Push<SpriteRenderer>         (m_VulkanState, m_VulkanDescriptorPool, m_VulkanDevice, m_RendererResourcePool)
			.Push<WorldPickRenderer>      (m_VulkanState, m_VulkanDescriptorPool, m_VulkanDevice, m_RendererResourcePool)
			.Push<WorldPickStage2Renderer>(m_VulkanState, m_VulkanDescriptorPool, m_VulkanDevice, m_RendererResourcePool)
			.Push<TestRenderer>           (m_VulkanState, m_VulkanDescriptorPool, m_VulkanDevice, m_RendererResourcePool)
			.Push<SlateRenderer>          (m_VulkanState, m_VulkanDescriptorPool, m_VulkanDevice, m_RendererResourcePool);
		}
	}

	VulkanRenderBackend::~VulkanRenderBackend()
	{
		SPIECS_PROFILE_ZONE;

		/**
		* @brief Release RendererResourcePool.
		*/
		m_RendererResourcePool = nullptr;

		/**
		* @brief Release VulkanDescriptorPool.
		*/
		m_VulkanDescriptorPool = nullptr;

		/**
		* @brief Release all Specific Renderer.
		*/
		{
			RendererManager::Get()
			.Pop("SlateRenderer")
			.Pop("TestRenderer")
			.Pop("WorldPickStage2Renderer")
			.Pop("WorldPickRenderer")
			.Pop("SpriteRenderer")
			.Pop("SceneComposeRenderer")
			.Pop("ShadowRenderer")
			.Pop("BasePassRenderer")
			.Pop("PreRenderer");
		}
	}

	void VulkanRenderBackend::RecreateSwapChain() 
	{
		SPIECS_PROFILE_ZONE;

		/**
		* @brief Get new size of Windows.
		*/
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

	void VulkanRenderBackend::CreateBottomLevelAS(FrameInfo& frameInfo)
	{
		std::vector<VulkanRayTracing::BlasInput> allBlas;

		/**
		* @brief Iter all MeshComponents.
		*/
		auto& view = frameInfo.m_World->GetRegistry().view<MeshComponent>();
		for (auto& e : view)
		{
			auto& meshComp = frameInfo.m_World->GetRegistry().get<MeshComponent>(e);

			auto blas = meshComp.GetMesh()->CreateMeshPackASInput();
			ContainerLibrary::Append<VulkanRayTracing::BlasInput>(allBlas, blas);
		}

		/**
		* @brief Build BLAS.
		*/
		//m_VulkanRayTracing->BuildBLAS(allBlas, VK_BUILD_ACCELERATION_STRUCTURE_PREFER_FAST_TRACE_BIT_KHR);
	}

	void VulkanRenderBackend::CreateTopLevelAS()
	{
		std::vector<VkAccelerationStructureInstanceKHR> tlas;
		tlas.reserve(1);

		{
			VkAccelerationStructureInstanceKHR rayInst{};
			//rayInst.transform                                  = nvvk::toTransformMatrixKHR(inst.transform);                 // Position of the instance
			rayInst.instanceCustomIndex                        = 0;                                                          // gl_InstanceCustomIndexEXT
			//rayInst.accelerationStructureReference             = m_VulkanRayTracing->GetBlasDeviceAddress(0);
			rayInst.flags                                      = VK_GEOMETRY_INSTANCE_TRIANGLE_FACING_CULL_DISABLE_BIT_KHR;
			rayInst.mask                                       = 0xFF;                                                       //  Only be hit if rayMask & instance.mask != 0
			rayInst.instanceShaderBindingTableRecordOffset     = 0;                                                          // We will use the same hit group for all objects

			tlas.emplace_back(rayInst);
		}

		/**
		* @brief Build TLAS.
		*/
		//m_VulkanRayTracing->BuildTLAS(tlas, VK_BUILD_ACCELERATION_STRUCTURE_PREFER_FAST_TRACE_BIT_KHR);
	}

	void VulkanRenderBackend::BeginFrame(FrameInfo& frameInfo)
	{
		SPIECS_PROFILE_ZONE;

		/**
		* @brief Wait for last frame done.
		*/
		vkWaitForFences(
			m_VulkanState.m_Device, 
			1, 
			&m_VulkanState.m_Fence[frameInfo.m_FrameIndex], 
			VK_TRUE, UINT64_MAX
		);

		/**
		* @brief Reset Fences.
		*/
		vkResetFences(
			m_VulkanState.m_Device, 
			1, 
			&m_VulkanState.m_Fence[frameInfo.m_FrameIndex]
		);

		/**
		* @brief Prepare Writing another SwapchainImage.
		*/
		VkResult result = vkAcquireNextImageKHR(
			m_VulkanState.m_Device, 
			m_VulkanState.m_SwapChain, 
			UINT64_MAX, 
			m_VulkanState.m_ImageSemaphore[frameInfo.m_FrameIndex], 
			VK_NULL_HANDLE, 
			&frameInfo.m_Imageindex
		);

		/**
		* @brief Confine whether swapchain need recreated.
		*/
		if (result == VK_ERROR_OUT_OF_DATE_KHR) 
		{
			RecreateSwapChain();
		}
		else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) 
		{
			SPIECS_CORE_ERROR("Failed to acquire swap chain image!");
		}

		/**
		* @brief Instance a VkCommandBufferBeginInfo.
		*/
		VkCommandBufferBeginInfo beginInfo{};
		beginInfo.sType            = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		beginInfo.flags            = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
		beginInfo.pInheritanceInfo = nullptr;

		/**
		* @brief Start recording a CommandBuffer.
		*/
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
			ImVec2 viewPortSize = SlateSystem::GetRegister()->GetViewPort()->GetPanelSize();

			viewport.y      =  viewPortSize.y;
			viewport.width  =  viewPortSize.x;
			viewport.height = -viewPortSize.y;
		}

		/**
		* @brief Set VkViewport with viewport slate.
		*/
		vkCmdSetViewport(m_VulkanState.m_CommandBuffer[frameInfo.m_FrameIndex], 0, 1, &viewport);

		/**
		* @brief Instance a VkRect2D
		*/
		VkRect2D scissor{};
		scissor.offset = { 0, 0 };
		scissor.extent = m_VulkanDevice->GetSwapChainSupport().surfaceSize;

		/**
		* @brief Set VkRect2D.
		*/
		vkCmdSetScissor(m_VulkanState.m_CommandBuffer[frameInfo.m_FrameIndex], 0, 1, &scissor);
	}

	void VulkanRenderBackend::EndFrame(FrameInfo& frameInfo)
	{
		SPIECS_PROFILE_ZONE;

		/**
		* @brief End recording the CommandBuffer.
		*/
		VK_CHECK(vkEndCommandBuffer(m_VulkanState.m_CommandBuffer[frameInfo.m_FrameIndex]));

		/**
		* @brief Reset Fences.
		*/
		vkResetFences(
			m_VulkanState.m_Device, 
			1, 
			&m_VulkanState.m_Fence[frameInfo.m_FrameIndex]
		);

		/**
		* @brief Instance a VkSubmitInfo.
		*/
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

		/**
		* @brief Submit all commands recorded to graphic queue.
		*/
		VK_CHECK(vkQueueSubmit(m_VulkanState.m_GraphicQueue, 1, &submitInfo, m_VulkanState.m_Fence[frameInfo.m_FrameIndex]));

		/**
		* @brief Instance a VkPresentInfoKHR.
		*/
		VkPresentInfoKHR presentInfo{};
		presentInfo.sType               = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
		presentInfo.waitSemaphoreCount  = 1;
		presentInfo.pWaitSemaphores     = signalSemaphores;

		VkSwapchainKHR swapChains[]     = { m_VulkanState.m_SwapChain };
		presentInfo.swapchainCount      = 1;
		presentInfo.pSwapchains         = swapChains;
		presentInfo.pImageIndices       = &frameInfo.m_Imageindex;

		presentInfo.pResults = nullptr;

		/**
		* @brief Present the swapchain image to windows.
		*/
		VkResult result = vkQueuePresentKHR(m_VulkanState.m_PresentQueue, &presentInfo);
		if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || m_VulkanWindows->IsResized()) 
		{
			m_VulkanWindows->SetResized(false);
			RecreateSwapChain();
		}
		else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
			SPIECS_CORE_ERROR("Failed to present swap chain image!");
		}
	}

	void VulkanRenderBackend::DrawTest(TimeStep& ts, FrameInfo& frameInfo)
	{
		SPIECS_PROFILE_ZONE;

		CreateBottomLevelAS(frameInfo);
		CreateTopLevelAS();

		/**
		* @brief Run all specific renderer.
		*/
		RendererManager::Run(ts, frameInfo);
	}

	void VulkanRenderBackend::OnEvent(Event& event)
	{
		SPIECS_PROFILE_ZONE;

		EventDispatcher dispatcher(event);

		dispatcher.Dispatch<WindowResizeOverEvent>(BIND_EVENT_FN(VulkanRenderBackend::OnWindowResizeOver));
		dispatcher.Dispatch<SlateResizeEvent>     (BIND_EVENT_FN(VulkanRenderBackend::OnSlateResize));
	}

	bool VulkanRenderBackend::OnWindowResizeOver(WindowResizeOverEvent& event)
	{
		SPIECS_PROFILE_ZONE;

		m_VulkanDevice->RequerySwapChainSupport();

		/**
		* @brief Recreate Swapchain here.
		*/
		m_VulkanSwapChain->Destroy();
		m_VulkanSwapChain->Create();

		/**
		* @brief Recreate all rendererpass which size is determained by viewportsize.
		*/
		RendererManager::Get().OnWindowResizeOver();

		/**
		* @brief Do not block the event.
		*/
		return false;
	}

	bool VulkanRenderBackend::OnSlateResize(SlateResizeEvent& event)
	{
		SPIECS_PROFILE_ZONE;

		/**
		* @brief Recreate all resources which size is determained by viewportsize.
		*/
		m_RendererResourcePool->OnSlateResize(event.GetWidth(), event.GetHeight());

		/**
		* @brief Recreate all rendererpass which size is determained by viewportsize.
		*/
		RendererManager::Get().OnSlateResize();

		/**
		* @brief Do not block the event.
		*/
		return false;
	}
}