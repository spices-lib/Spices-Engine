/**
* @file VulkanUtils.h.
* @brief The VulkanObject Class Definitions.
* @author Spices.
*/

#pragma once
#include "Core/Core.h"
#include "VulkanDebugUtils.h"
#include "VulkanFunctions.h"
#include "vk_mem_alloc.h"

#include <vector>

namespace Spices {

/**
* @brief Max In Flight Frame.
* 2 buffers are enough in this program.
*/
#define MaxFrameInFlight 2

/**
* @brief Use VMA for memory allocate.
*/
#define VMA_ALLOCATOR

	/**
	* @brief This struct contains all Vulkan object in used golbal.
	*/
	struct VulkanState
	{
		/**
		* @brief Constructor Function.
		*/
		VulkanState() = default;

		/**
		* @brief Copy Constructor Function.
		* @note This Class not allowed copy behaves.
		*/
		VulkanState(const VulkanState&) = delete;

		/**
		* @brief Copy Assignment Operation.
		* @note This Class not allowed copy behaves.
		*/
		VulkanState& operator=(const VulkanState&) = delete;

		GLFWwindow*                                      m_Windows;                    /* @brief from VulkanWindows, defined by GLFW.                            */
		VkInstance                                       m_Instance;                   /* @brief From VulkanInstance.                                            */
		VkSurfaceKHR                                     m_Surface;                    /* @brief From VulkanInstance.                                            */
		VkPhysicalDevice                                 m_PhysicalDevice;             /* @brief From VulkanDevice.                                              */
		VkDevice                                         m_Device;                     /* @brief From VulkanDevice.                                              */
		VmaAllocator                                     m_VmaAllocator;               /* @brief From VulkanMemoryAllocator.                                     */
		VulkanFunctions                                  m_VkFunc;                     /* @brief From VulkanFunctions.                                           */
		VkQueue											 m_GraphicQueue;               /* @brief From VulkanDevice, Queue for graphic compute.                   */
		uint32_t                                         m_GraphicQueueFamily;         /* @brief From VulkanDevice, ID for graphic queue.                        */
		VkQueue											 m_PresentQueue;               /* @brief From VulkanDevice, Queue for present windows.                   */
		VkQueue											 m_TransferQueue;              /* @brief From VulkanDevice, Queue for transfer buffer.                   */
		VkQueue											 m_ComputeQueue;               /* @brief From VulkanDevice, Queue for compute shader.                    */
		uint32_t                                         m_ComputeQueueFamily;         /* @brief From VulkanDevice, ID for compute queue.                        */
		VkSwapchainKHR                                   m_SwapChain;                  /* @brief From VulkanSwapChain.                                           */
		std::array<VkImage,           MaxFrameInFlight>  m_SwapChainImages;            /* @brief The SwapChain's image, used for present.                        */
		std::array<VkImageView,       MaxFrameInFlight>  m_SwapChainImageViews;        /* @brief The SwapChain's imageView.                                      */
		std::array<VkSampler,         MaxFrameInFlight>  m_SwapChainImageSamplers;     /* @brief The SwapChain's imageSampler.                                   */
	  
		// Used for Graphic Queue	  
		VkCommandPool                                    m_GraphicCommandPool;         /* @brief From VulkanCommandBuffer                                        */
		std::array<VkCommandBuffer,   MaxFrameInFlight>  m_GraphicCommandBuffer;       /* @brief From VulkanCommandBuffer, Array num equals to MaxFrameInFlight. */
		std::array<VkSemaphore,       MaxFrameInFlight>  m_GraphicImageSemaphore;      /* @brief From VulkanSwapChain, Array num equals to MaxFrameInFlight.     */
		std::array<VkSemaphore,       MaxFrameInFlight>  m_GraphicQueueSemaphore;      /* @brief From VulkanSwapChain, Array num equals to MaxFrameInFlight.     */
		std::array<VkFence,           MaxFrameInFlight>  m_GraphicFence;               /* @brief From VulkanSwapChain, Array num equals to MaxFrameInFlight.     */
									  
		// Use for Compute Queue.	  
		VkCommandPool                                    m_ComputeCommandPool;         /* @brief From VulkanCommandBuffer                                        */
		std::array<VkCommandBuffer,   MaxFrameInFlight>  m_ComputeCommandBuffer;       /* @brief From VulkanCommandBuffer, Array num equals to MaxFrameInFlight. */
		std::array<VkSemaphore,       MaxFrameInFlight>  m_ComputeImageSemaphore;      /* @brief From VulkanSwapChain, Array num equals to MaxFrameInFlight.     */     /* @note not in use. */
		std::array<VkSemaphore,       MaxFrameInFlight>  m_ComputeQueueSemaphore;      /* @brief From VulkanSwapChain, Array num equals to MaxFrameInFlight.     */
		std::array<VkFence,           MaxFrameInFlight>  m_ComputeFence;               /* @brief From VulkanSwapChain, Array num equals to MaxFrameInFlight.     */
	};

	/**
	* @brief VulkanObject Class.
	* This class defines the basic behaves of VulkanObject.
	* When we create an new VulkanObject, we need inherit from this.
	*/
	class VulkanObject
	{
	public:

		/**
		* @brief Constructor Function.
		* Init member variables.
		* @param[in] vulkanState The global VulkanState.
		*/
		VulkanObject(VulkanState& vulkanState) 
			: m_VulkanState(vulkanState)
		{}

		/**
		* @brief Destructor Function.
		* We destroy pipeline layout here.
		*/
		virtual ~VulkanObject() = default;

		/**
		* @brief Copy Constructor Function.
		* @note This Class not allowed copy behaves.
		*/
		VulkanObject(const VulkanObject&) = delete;

		/**
		* @brief Copy Assignment Operation.
		* @note This Class not allowed copy behaves.
		*/
		VulkanObject& operator=(const VulkanObject&) = delete;

	protected:

		/**
		* @brief The global VulkanState Referenced from VulkanRenderBackend.
		*/
		VulkanState& m_VulkanState;
	};

}