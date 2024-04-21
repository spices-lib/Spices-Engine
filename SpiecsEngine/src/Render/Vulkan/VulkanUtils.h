/**
* @file VulkanUtils.h.
* @brief The VulkanObject Class Definitions.
* @author Spiecs.
*/

#pragma once
#include "Core/Core.h"

#include <vector>

namespace Spiecs {

/**
* @brief Max In Flight Frame.
*/
#define MaxFrameInFlight 2

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
		* @note This Class not allowed copy behaver.
		*/
		VulkanState(const VulkanState&) = delete;

		/**
		* @brief Copy Assignment Operation.
		* @note This Class not allowed copy behaver.
		*/
		VulkanState& operator=(const VulkanState&) = delete;

		/**
		* @brief from VulkanWindows, defined by GLFW.
		*/
		GLFWwindow* m_Windows;

		/**
		* @brief From VulkanInstance.
		*/
		VkInstance m_Instance;

		/**
		* @brief From VulkanInstance.
		*/
		VkSurfaceKHR m_Surface;

		/**
		* @brief From VulkanDevice.
		*/
		VkPhysicalDevice m_PhysicalDevice;

		/**
		* @brief From VulkanDevice.
		*/
		VkDevice m_Device;

		/**
		* @brief From VulkanDevice.
		* Queue for graphic compute
		*/
		VkQueue m_GraphicQueue;

		/**
		* @brief From VulkanDevice.
		* Queue for present windows
		*/
		VkQueue m_PresentQueue;

		/**
		* @brief From VulkanDevice.
		* Queue for transfer buffer.
		*/
		VkQueue m_TransformQueue;

		/**
		* @brief From VulkanDevice.
		* Queue for compute shader.
		*/
		VkQueue m_ComputeQueue;

		/**
		* @brief From VulkanSwapChain.
		*/
		VkSwapchainKHR m_SwapChain;

		/**
		* @brief The SwapChain's image, used for present.
		*/
		std::array<VkImage, MaxFrameInFlight> m_SwapChainImages;

		/**
		* @brief The SwapChain's imageView.
		*/
		std::array<VkImageView, MaxFrameInFlight> m_SwapChainImageViews;

		/**
		* @brief The SwapChain's imageSampler.
		*/
		std::array<VkSampler, MaxFrameInFlight> m_SwapChainImageSamplers;

		/**
		* @brief From VulkanCommandBuffer
		*/
		VkCommandPool m_CommandPool;

		/**
		* @brief From VulkanCommandBuffer
		* Array num equals to MaxFrameInFlight
		*/
		std::vector<VkCommandBuffer> m_CommandBuffer;

		/**
		* @brief From VulkanSwapChain
		* Array num equals to MaxFrameInFlight
		*/
		std::vector<VkSemaphore> m_ImageSemaphore;

		/**
		* @brief From VulkanSwapChain
		* Array num equals to MaxFrameInFlight
		*/
		std::vector<VkSemaphore> m_QueueSemaphore;

		/**
		* @brief From VulkanSwapChain
		* Array num equals to MaxFrameInFlight
		*/
		std::vector<VkFence> m_Fence;
	};

	/**
	* @brief VulkanObject Class.
	* This class defines the basic behaver of VulkanObject.
	* When we create an new VulkanObject, we need inherit from this.
	*/
	class VulkanObject
	{
	public:

		/**
		* @brief Constructor Function.
		* Init member veriables.
		* @param[in] vulkanState The global VulkanState.
		*/
		VulkanObject(VulkanState& vulkanState) : m_VulkanState(vulkanState){};

		/**
		* @brief Destructor Function.
		* We destroy pipeline layout here.
		*/
		virtual ~VulkanObject() {};

		/**
		* @brief Copy Constructor Function.
		* @note This Class not allowed copy behaver.
		*/
		VulkanObject(const VulkanObject&) = delete;

		/**
		* @brief Copy Assignment Operation.
		* @note This Class not allowed copy behaver.
		*/
		VulkanObject& operator=(const VulkanObject&) = delete;

	protected:

		/**
		* @brief The global VulkanState Referenced from VulkanRenderBackend.
		*/
		VulkanState& m_VulkanState;
	};

}