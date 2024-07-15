/**
* @file VulkanRenderBackend_test.h.
* @brief The VulkanRenderBackend_test Definitions.
* @author Spices.
*/

#pragma once
#include <gmock/gmock.h>
#include <Render/Vulkan/VulkanRenderBackend.h>

// Dependency
#include <Resources/ResourcePool/ResourcePool.h>
#include <Resources/Material/Material.h>
#include <Resources/Mesh/Mesh.h>
#include <Resources/Texture/Texture.h>

namespace SpicesTest {

	/**
	* @brief Unit Test for VulkanRenderBackend.
	*/
	class VulkanRenderBackend_test : public testing::Test
	{
	protected:

		/**
		* @brief The interface is inherited from testing::Test.
		* Registy on Initialize.
		*/
		virtual void SetUp() override {

			/**
			* @brief VulkanRenderBackend Dependency 1 : Spices::Log.
			*/
			Spices::Log::Init();

			/**
			* @brief Instance a VulkanRenderBackend.
			*/
			m_RenderBackend = std::make_unique<Spices::VulkanRenderBackend>();
		}

		/**
		* @brief The interface is inherited from testing::Test.
		* Registy on ShutDown.
		*/
		virtual void TearDown() override {

			/**
			* @brief VulkanRenderBackend Dependency 2 : Spices::ResourcePool.
			*/
			Spices::ResourcePool<Spices::Texture>  ::Destroy();
			Spices::ResourcePool<Spices::Material> ::Destroy();
			Spices::ResourcePool<Spices::Mesh>     ::Destroy();
		};

	protected:

		/**
		* @brief The unique pointer of VulkanRenderBackend.
		*/
		std::unique_ptr<Spices::VulkanRenderBackend> m_RenderBackend;
	};

	/**
	* @brief Testing VulkanRenderBackend Initialize.
	*/
	TEST_F(VulkanRenderBackend_test, Initialize) {

		EXPECT_NE(m_RenderBackend,                                                 nullptr            );    /*@see Spices::VulkanRenderBackend*/

		EXPECT_NE(m_RenderBackend->GetState().m_Windows,                           nullptr            );    /*@see Spices::VulkanWindows*/
		EXPECT_NE(m_RenderBackend->GetState().m_Instance,                          nullptr            );    /*@see Spices::VulkanInstance*/
		EXPECT_NE(m_RenderBackend->GetState().m_Surface,                           nullptr            );    /*@see Spices::VkSurfaceKHR*/
		EXPECT_NE(m_RenderBackend->GetState().m_PhysicalDevice,                    nullptr            );    /*@see Spices::VkPhysicalDevice*/
		EXPECT_NE(m_RenderBackend->GetState().m_Device,                            nullptr            );    /*@see Spices::VkDevice*/
		EXPECT_NE(m_RenderBackend->GetState().m_GraphicQueue,                      nullptr            );    /*@see Spices::VkQueue*/
		EXPECT_NE(m_RenderBackend->GetState().m_PresentQueue,                      nullptr            );    /*@see Spices::VkQueue*/
		EXPECT_NE(m_RenderBackend->GetState().m_TransformQueue,                    nullptr            );    /*@see Spices::VkQueue*/
		EXPECT_NE(m_RenderBackend->GetState().m_ComputeQueue,                      nullptr            );    /*@see Spices::VkQueue*/
		EXPECT_NE(m_RenderBackend->GetState().m_SwapChain,                         nullptr            );    /*@see Spices::VkSwapchainKHR*/
		EXPECT_NE(m_RenderBackend->GetState().m_SwapChainImages.data(),            nullptr            );    /*@see Spices::VkImage*/
		EXPECT_EQ(m_RenderBackend->GetState().m_SwapChainImages.size(),            MaxFrameInFlight   );    /*@see Spices::VkImage*/
		EXPECT_NE(m_RenderBackend->GetState().m_SwapChainImageSamplers.data(),     nullptr            );    /*@see Spices::VkSampler*/
		EXPECT_EQ(m_RenderBackend->GetState().m_SwapChainImageSamplers.size(),     MaxFrameInFlight   );    /*@see Spices::VkSampler*/
		EXPECT_NE(m_RenderBackend->GetState().m_SwapChainImageViews.data(),        nullptr            );    /*@see Spices::VkImageView*/ 
		EXPECT_EQ(m_RenderBackend->GetState().m_SwapChainImageViews.size(),        MaxFrameInFlight   );    /*@see Spices::VkImageView*/
		EXPECT_NE(m_RenderBackend->GetState().m_ImageSemaphore.data(),             nullptr            );    /*@see Spices::VkSemaphore*/    
		EXPECT_EQ(m_RenderBackend->GetState().m_ImageSemaphore.size(),             MaxFrameInFlight   );    /*@see Spices::VkSemaphore*/
		EXPECT_NE(m_RenderBackend->GetState().m_QueueSemaphore.data(),             nullptr            );    /*@see Spices::VkSemaphore*/   
		EXPECT_EQ(m_RenderBackend->GetState().m_QueueSemaphore.size(),             MaxFrameInFlight   );    /*@see Spices::VkSemaphore*/
		EXPECT_NE(m_RenderBackend->GetState().m_Fence.data(),                      nullptr            );    /*@see Spices::VkFence*/    
		EXPECT_EQ(m_RenderBackend->GetState().m_Fence.size(),                      MaxFrameInFlight   );    /*@see Spices::VkFence*/
		EXPECT_NE(m_RenderBackend->GetState().m_CommandPool,                       nullptr            );    /*@see Spices::VkCommandPool*/    
		EXPECT_NE(m_RenderBackend->GetState().m_CommandBuffer.data(),              nullptr            );    /*@see Spices::VkCommandBuffer*/    
		EXPECT_EQ(m_RenderBackend->GetState().m_CommandBuffer.size(),              MaxFrameInFlight   );    /*@see Spices::VkCommandBuffer*/
																										    
		EXPECT_NE(m_RenderBackend->GetDescriptorPool(),                            nullptr            );    /*@see Spices::VulkanDescriptorPool*/
		EXPECT_NE(m_RenderBackend->GetRendererResourcePool(),                      nullptr            );    /*@see Spices::RendererResourcePool*/
	}
}