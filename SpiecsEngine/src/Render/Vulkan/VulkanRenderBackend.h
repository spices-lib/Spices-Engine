#pragma once
#include "Core/Core.h"
#include "VulkanUtils.h"
#include "Render/FrameInfo.h"

#include "VulkanWindows.h"
#include "VulkanInstance.h"
#include "VulkanDevice.h"
#include "VulkanCommandBuffer.h"
#include "VulkanRenderPass.h"
#include "VulkanSwapChain.h"
#include "VulkanPipeline.h"
#include "VulkanDescriptor.h"

namespace Spiecs {

	class VulkanRenderBackend
	{
	public:
		VulkanRenderBackend();
		virtual ~VulkanRenderBackend();

		VulkanRenderBackend(const VulkanRenderBackend&) = delete;
		VulkanRenderBackend& operator=(const VulkanRenderBackend&) = delete;

		bool isWindowClosed() { return glfwWindowShouldClose(m_VulkanState.m_Windows); };
		void WaitIdle() { vkDeviceWaitIdle(m_VulkanState.m_Device); };

		void beginFrame(FrameInfo& frameInfo);
		void endFrame(FrameInfo& frameInfo);

		void DrawTest(FrameInfo& frameInfo);

		// Temp:
		VulkanState& GetState() { return m_VulkanState; };

	private:
		void RecreateSwapChain();
	private:
		static VulkanState m_VulkanState;

		std::unique_ptr<VulkanWindows> m_VulkanWindows;
		std::unique_ptr<VulkanInstance> m_VulkanInstance;
		std::shared_ptr<VulkanDevice> m_VulkanDevice;
		std::unique_ptr<VulkanCommandPool> m_VulkanCommandPool;
		std::unique_ptr<VulkanCommandBuffer> m_VulkanCommandBuffer;
		std::unique_ptr<VulkanRenderPass> m_VulkanRenderPass;
		std::unique_ptr<VulkanSwapChain> m_VulkanSwapChain;
		std::unique_ptr<VulkanDescriptorPool> m_VulkanDescriptorPool;
	};
}