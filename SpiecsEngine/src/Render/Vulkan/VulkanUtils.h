#include "Core/Core.h"

#include <vector>

namespace Spiecs {

	struct VulkanState
	{
		GLFWwindow* m_Windows;
		VkInstance m_Instance;
		VkSurfaceKHR m_Surface;
		VkPhysicalDevice m_PhysicalDevice;
		VkDevice m_Device;

		VkQueue m_GraphicQueue;
		VkQueue m_PresentQueue;
		VkQueue m_TransformQueue;
		VkQueue m_ComputeQueue;

		VkSwapchainKHR m_SwapChain;
		VkCommandPool m_CommandPool;

		std::vector<VkCommandBuffer> m_CommandBuffer;
		std::vector<VkSemaphore> m_ImageSemaphore;
		std::vector<VkSemaphore> m_FrameSemaphore;
		std::vector<VkFence> m_Fence;
	};

	VulkanState m_VulkanState{};
}