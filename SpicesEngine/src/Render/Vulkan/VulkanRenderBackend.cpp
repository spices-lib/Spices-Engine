/**
* @file VulkanRenderBackend.cpp.
* @brief The VulkanRenderBackend Class Implementation.
* @author Spices.
*/

#include "Pchheader.h"
#include "VulkanRenderBackend.h"

#include "Render/RendererResource/RendererResourcePool.h"
#include "Systems/SlateSystem.h"

#include "Render/Renderer/SpecificRenderer/PreRenderer.h"
#include "Render/Renderer/SpecificRenderer/RayTracingRenderer.h"
#include "Render/Renderer/SpecificRenderer/BasePassRenderer.h"
#include "Render/Renderer/SpecificRenderer/ShadowRenderer.h"
#include "Render/Renderer/SpecificRenderer/SlateRenderer.h"
#include "Render/Renderer/SpecificRenderer/SceneComposeRenderer.h"
#include "Render/Renderer/SpecificRenderer/WorldPickRenderer.h"
#include "Render/Renderer/SpecificRenderer/SpriteRenderer.h"
#include "Render/Renderer/SpecificRenderer/WorldPickStage2Renderer.h"
#include "Render/Renderer/SpecificRenderer/ParticleRenderer.h"
#include "Render/Renderer/SpecificRenderer/TestRenderer.h"
#include "Render/Renderer/SpecificRenderer/RayTracingComposeRenderer.h"

namespace Spices {

	/**
	* @brief Window create parameter.
	*/
	const WindowInfo initInfo{ 2200, 1300, "Spices Engine", std::make_shared<WindowIcon>(SPICES_ENGINE_ASSETS_PATH + "Textures/src/Hello_Spices_Engine.png")};

	VulkanState                           VulkanRenderBackend::m_VulkanState;
	std::shared_ptr<VulkanDescriptorPool> VulkanRenderBackend::m_VulkanDescriptorPool;
	std::shared_ptr<RendererResourcePool> VulkanRenderBackend::m_RendererResourcePool;

	VulkanRenderBackend::VulkanRenderBackend()
	{
		SPICES_PROFILE_ZONE;

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
		}

		/**
		* @brief Create VulkanDescriptorPool.
		*/
		{
			m_VulkanDescriptorPool = VulkanDescriptorPool::Builder()
			.SetPoolFlags(VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT | VK_DESCRIPTOR_POOL_CREATE_UPDATE_AFTER_BIND_BIT)
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

			/* @brief Ray Tracing Renderer */
			.Push<RayTracingRenderer>     (m_VulkanState, m_VulkanDescriptorPool, m_VulkanDevice, m_RendererResourcePool)
			.Push<RayTracingComposeRenderer>(m_VulkanState, m_VulkanDescriptorPool, m_VulkanDevice, m_RendererResourcePool)

			/* @brief Rasterization Renderer */
			.Push<BasePassRenderer>       (m_VulkanState, m_VulkanDescriptorPool, m_VulkanDevice, m_RendererResourcePool)
			//.Push<ShadowRenderer>         (m_VulkanState, m_VulkanDescriptorPool, m_VulkanDevice, m_RendererResourcePool)
			.Push<SceneComposeRenderer>   (m_VulkanState, m_VulkanDescriptorPool, m_VulkanDevice, m_RendererResourcePool)
			//.Push<SpriteRenderer>         (m_VulkanState, m_VulkanDescriptorPool, m_VulkanDevice, m_RendererResourcePool)
			//.Push<WorldPickRenderer>      (m_VulkanState, m_VulkanDescriptorPool, m_VulkanDevice, m_RendererResourcePool)
			//.Push<WorldPickStage2Renderer>(m_VulkanState, m_VulkanDescriptorPool, m_VulkanDevice, m_RendererResourcePool)
			//.Push<ParticleRenderer>       (m_VulkanState, m_VulkanDescriptorPool, m_VulkanDevice, m_RendererResourcePool)
			//.Push<TestRenderer>           (m_VulkanState, m_VulkanDescriptorPool, m_VulkanDevice, m_RendererResourcePool)
			.Push<SlateRenderer>          (m_VulkanState, m_VulkanDescriptorPool, m_VulkanDevice, m_RendererResourcePool);
		}
	}

	VulkanRenderBackend::~VulkanRenderBackend()
	{
		SPICES_PROFILE_ZONE;

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
			//.Pop("TestRenderer")
			//.Pop("ParticleRenderer")
			//.Pop("WorldPickStage2Renderer")
			//.Pop("WorldPickRenderer")
			//.Pop("SpriteRenderer")
			.Pop("SceneComposeRenderer")
			//.Pop("ShadowRenderer")
			.Pop("BasePassRenderer")
			.Pop("RayTracingComposeRenderer")
			.Pop("RayTracingRenderer")
			.Pop("PreRenderer");
		}
	}

	void VulkanRenderBackend::RecreateSwapChain()
	{
		SPICES_PROFILE_ZONE;

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

	void VulkanRenderBackend::BeginFrame(FrameInfo& frameInfo)
	{
		SPICES_PROFILE_ZONE;

		VkFence fence[2] = { m_VulkanState.m_ComputeFence[frameInfo.m_FrameIndex], m_VulkanState.m_GraphicFence[frameInfo.m_FrameIndex] };

		{
			SPICES_PROFILE_ZONEN("BeginFrame::WaitForFences");
			
			/**
			* @brief Wait for last frame done.
			*/
			VK_CHECK(vkWaitForFences(m_VulkanState.m_Device, 2, fence, VK_TRUE, UINT64_MAX))
		}

		{
			SPICES_PROFILE_ZONEN("BeginFrame::ResetFences");
			
			/**
			* @brief Reset Fences.
			*/
			VK_CHECK(vkResetFences(m_VulkanState.m_Device, 2, fence));
		}
		
		/**
		* @brief Prepare Writing another SwapchainImage.
		*/
		const VkResult result = vkAcquireNextImageKHR(
			m_VulkanState.m_Device, 
			m_VulkanState.m_SwapChain, 
			UINT64_MAX, 
			m_VulkanState.m_GraphicImageSemaphore[frameInfo.m_FrameIndex],
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
			SPICES_CORE_ERROR("Failed to acquire swap chain image!");
		}

		{
			SPICES_PROFILE_ZONEN("BeginFrame::BeginCommandBuffer");
			
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
			VK_CHECK(vkBeginCommandBuffer(m_VulkanState.m_ComputeCommandBuffer[frameInfo.m_FrameIndex], &beginInfo))
			VK_CHECK(vkBeginCommandBuffer(m_VulkanState.m_GraphicCommandBuffer[frameInfo.m_FrameIndex], &beginInfo))
		}
	}

	void VulkanRenderBackend::EndFrame(FrameInfo& frameInfo)
	{
		SPICES_PROFILE_ZONE;

		{
			SPICES_PROFILE_ZONEN("EndFrame::EndCommandBuffer");
			
			/**
			* @brief End recording the CommandBuffer.
			*/
			VK_CHECK(vkEndCommandBuffer(m_VulkanState.m_ComputeCommandBuffer[frameInfo.m_FrameIndex]))
			VK_CHECK(vkEndCommandBuffer(m_VulkanState.m_GraphicCommandBuffer[frameInfo.m_FrameIndex]))
		}

		{
			SPICES_PROFILE_ZONEN("EndFrame::ResetFences");
			
			/**
			* @brief Reset Fences.
			*/
			VkFence fence[2] = { m_VulkanState.m_ComputeFence[frameInfo.m_FrameIndex], m_VulkanState.m_GraphicFence[frameInfo.m_FrameIndex] };
			VK_CHECK(vkResetFences(m_VulkanState.m_Device, 1, fence))
		}
		
		{
			SPICES_PROFILE_ZONEN("EndFrame::ComputeQueueSubmit");

			/**
			* @brief Instance a VkSubmitInfo.
			*/
			VkSubmitInfo submitInfo{};
			submitInfo.sType                    = VK_STRUCTURE_TYPE_SUBMIT_INFO;

			VkSemaphore waitSemphores[]         = { m_VulkanState.m_GraphicQueueSemaphore[frameInfo.m_FrameIndex]};
			VkPipelineStageFlags waitStages[]   = { VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT };

			submitInfo.waitSemaphoreCount       = 0;
			submitInfo.pWaitSemaphores          = nullptr;
			submitInfo.pWaitDstStageMask        = waitStages;
			submitInfo.commandBufferCount       = 1;
			submitInfo.pCommandBuffers          = &m_VulkanState.m_ComputeCommandBuffer[frameInfo.m_FrameIndex];
							    
			VkSemaphore signalSemaphores[]      = { m_VulkanState.m_ComputeQueueSemaphore[frameInfo.m_FrameIndex] };
			submitInfo.signalSemaphoreCount     = 1;
			submitInfo.pSignalSemaphores        = signalSemaphores;
			
			/**
			* @brief Submit all commands recorded in queue.
			*/
			VK_CHECK(vkQueueSubmit(m_VulkanState.m_ComputeQueue, 1, &submitInfo, m_VulkanState.m_ComputeFence[frameInfo.m_FrameIndex]))
		}

		{
			SPICES_PROFILE_ZONEN("EndFrame::GraphicQueueSubmit");

			/**
			* @brief Instance a VkSubmitInfo.
			*/
			VkSubmitInfo                          submitInfo{};
			submitInfo.sType                    = VK_STRUCTURE_TYPE_SUBMIT_INFO;
										        
			VkSemaphore waitSemphores[]         = { m_VulkanState.m_ComputeQueueSemaphore[frameInfo.m_FrameIndex], m_VulkanState.m_GraphicImageSemaphore[frameInfo.m_FrameIndex] };
			VkPipelineStageFlags waitStages[]   = { VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT, VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };

			submitInfo.waitSemaphoreCount       = 2;
			submitInfo.pWaitSemaphores          = waitSemphores;
			submitInfo.pWaitDstStageMask        = waitStages;
			submitInfo.commandBufferCount       = 1;
			submitInfo.pCommandBuffers          = &m_VulkanState.m_GraphicCommandBuffer[frameInfo.m_FrameIndex];

			VkSemaphore signalSemaphores[]      = { m_VulkanState.m_GraphicQueueSemaphore[frameInfo.m_FrameIndex] };
			submitInfo.signalSemaphoreCount     = 1;
			submitInfo.pSignalSemaphores        = signalSemaphores;

			/**
			* @brief Submit all commands recorded in queue.
			*/
			VK_CHECK(vkQueueSubmit(m_VulkanState.m_GraphicQueue, 1, &submitInfo, m_VulkanState.m_GraphicFence[frameInfo.m_FrameIndex]))
		}

		{
			SPICES_PROFILE_ZONEN("EndFrame::QueuePresent");
			
			/**
			* @brief Instance a VkPresentInfoKHR.
			*/
			VkPresentInfoKHR presentInfo{};
			presentInfo.sType               = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
			presentInfo.waitSemaphoreCount  = 1;
			presentInfo.pWaitSemaphores     = &m_VulkanState.m_GraphicQueueSemaphore[frameInfo.m_FrameIndex];

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
				SPICES_CORE_ERROR("Failed to present swap chain image!");
			}
		}
	}

	void VulkanRenderBackend::DrawTest(TimeStep& ts, FrameInfo& frameInfo)
	{
		SPICES_PROFILE_ZONE;

		/**
		* @brief Run all specific renderer.
		*/
		RendererManager::Run(ts, frameInfo);
	}

	void VulkanRenderBackend::OnEvent(Event& event)
	{
		SPICES_PROFILE_ZONE;

		EventDispatcher dispatcher(event);

		dispatcher.Dispatch<WindowResizeOverEvent>(BIND_EVENT_FN(VulkanRenderBackend::OnWindowResizeOver));
		dispatcher.Dispatch<SlateResizeEvent>     (BIND_EVENT_FN(VulkanRenderBackend::OnSlateResize));
		dispatcher.Dispatch<MeshAddedWorldEvent>  (BIND_EVENT_FN(VulkanRenderBackend::OnMeshAddedWorldEvent));
	}

	bool VulkanRenderBackend::OnWindowResizeOver(WindowResizeOverEvent& event)
	{
		SPICES_PROFILE_ZONE;

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
		SPICES_PROFILE_ZONE;

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

	bool VulkanRenderBackend::OnMeshAddedWorldEvent(WorldEvent& event)
	{
		SPICES_PROFILE_ZONE;

		/**
		* @brief Recreate descriptorset or other.
		*/
		RendererManager::Get().OnMeshAddedWorld();

		return false;
	}
}