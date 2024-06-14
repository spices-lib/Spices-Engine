/**
* @file VulkanRenderBackend_test.h.
* @brief The VulkanRenderBackend_test Definitions.
* @author Spiecs.
*/

#pragma once
#include <gmock/gmock.h>
#include <Render/Vulkan/VulkanRenderBackend.h>

// Dependency
#include <Resources/ResourcePool/ResourcePool.h>
#include <Resources/Material/Material.h>
#include <Resources/Mesh/Mesh.h>
#include <Resources/Texture/Texture.h>

namespace SpiecsTest {

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
			* @brief VulkanRenderBackend Dependency 1 : Spiecs::Log.
			*/
			Spiecs::Log::Init();

			/**
			* @brief Instance a VulkanRenderBackend.
			*/
			m_RenderBackend = std::make_unique<Spiecs::VulkanRenderBackend>();
		}

		/**
		* @brief The interface is inherited from testing::Test.
		* Registy on ShutDown.
		*/
		virtual void TearDown() override {

			/**
			* @brief VulkanRenderBackend Dependency 2 : Spiecs::ResourcePool.
			*/
			Spiecs::ResourcePool<Spiecs::Texture>  ::Destroy();
			Spiecs::ResourcePool<Spiecs::Material> ::Destroy();
			Spiecs::ResourcePool<Spiecs::Mesh>     ::Destroy();
		};

	protected:

		/**
		* @brief The unique pointer of VulkanRenderBackend.
		*/
		std::unique_ptr<Spiecs::VulkanRenderBackend> m_RenderBackend;
	};

	/**
	* @brief Testing VulkanRenderBackend Initialize.
	*/
	TEST_F(VulkanRenderBackend_test, Initialize) {

		EXPECT_NE(m_RenderBackend,                                                 nullptr            );    /*@see Spiecs::VulkanRenderBackend*/

		EXPECT_NE(m_RenderBackend->GetState().m_Windows,                           nullptr            );    /*@see Spiecs::VulkanWindows*/
		EXPECT_NE(m_RenderBackend->GetState().m_Instance,                          nullptr            );    /*@see Spiecs::VulkanInstance*/
		EXPECT_NE(m_RenderBackend->GetState().m_Surface,                           nullptr            );    /*@see Spiecs::VkSurfaceKHR*/
		EXPECT_NE(m_RenderBackend->GetState().m_PhysicalDevice,                    nullptr            );    /*@see Spiecs::VkPhysicalDevice*/
		EXPECT_NE(m_RenderBackend->GetState().m_Device,                            nullptr            );    /*@see Spiecs::VkDevice*/
		EXPECT_NE(m_RenderBackend->GetState().m_GraphicQueue,                      nullptr            );    /*@see Spiecs::VkQueue*/
		EXPECT_NE(m_RenderBackend->GetState().m_PresentQueue,                      nullptr            );    /*@see Spiecs::VkQueue*/
		EXPECT_NE(m_RenderBackend->GetState().m_TransformQueue,                    nullptr            );    /*@see Spiecs::VkQueue*/
		EXPECT_NE(m_RenderBackend->GetState().m_ComputeQueue,                      nullptr            );    /*@see Spiecs::VkQueue*/
		EXPECT_NE(m_RenderBackend->GetState().m_SwapChain,                         nullptr            );    /*@see Spiecs::VkSwapchainKHR*/
		EXPECT_NE(m_RenderBackend->GetState().m_SwapChainImages.data(),            nullptr            );    /*@see Spiecs::VkImage*/
		EXPECT_EQ(m_RenderBackend->GetState().m_SwapChainImages.size(),            MaxFrameInFlight   );    /*@see Spiecs::VkImage*/
		EXPECT_NE(m_RenderBackend->GetState().m_SwapChainImageSamplers.data(),     nullptr            );    /*@see Spiecs::VkSampler*/
		EXPECT_EQ(m_RenderBackend->GetState().m_SwapChainImageSamplers.size(),     MaxFrameInFlight   );    /*@see Spiecs::VkSampler*/
		EXPECT_NE(m_RenderBackend->GetState().m_SwapChainImageViews.data(),        nullptr            );    /*@see Spiecs::VkImageView*/ 
		EXPECT_EQ(m_RenderBackend->GetState().m_SwapChainImageViews.size(),        MaxFrameInFlight   );    /*@see Spiecs::VkImageView*/
		EXPECT_NE(m_RenderBackend->GetState().m_ImageSemaphore.data(),             nullptr            );    /*@see Spiecs::VkSemaphore*/    
		EXPECT_EQ(m_RenderBackend->GetState().m_ImageSemaphore.size(),             MaxFrameInFlight   );    /*@see Spiecs::VkSemaphore*/
		EXPECT_NE(m_RenderBackend->GetState().m_QueueSemaphore.data(),             nullptr            );    /*@see Spiecs::VkSemaphore*/   
		EXPECT_EQ(m_RenderBackend->GetState().m_QueueSemaphore.size(),             MaxFrameInFlight   );    /*@see Spiecs::VkSemaphore*/
		EXPECT_NE(m_RenderBackend->GetState().m_Fence.data(),                      nullptr            );    /*@see Spiecs::VkFence*/    
		EXPECT_EQ(m_RenderBackend->GetState().m_Fence.size(),                      MaxFrameInFlight   );    /*@see Spiecs::VkFence*/
		EXPECT_NE(m_RenderBackend->GetState().m_CommandPool,                       nullptr            );    /*@see Spiecs::VkCommandPool*/    
		EXPECT_NE(m_RenderBackend->GetState().m_CommandBuffer.data(),              nullptr            );    /*@see Spiecs::VkCommandBuffer*/    
		EXPECT_EQ(m_RenderBackend->GetState().m_CommandBuffer.size(),              MaxFrameInFlight   );    /*@see Spiecs::VkCommandBuffer*/
																										    
		EXPECT_NE(m_RenderBackend->GetDescriptorPool(),                            nullptr            );    /*@see Spiecs::VulkanDescriptorPool*/
		EXPECT_NE(m_RenderBackend->GetRendererResourcePool(),                      nullptr            );    /*@see Spiecs::RendererResourcePool*/
	}
}