/**
* @file VulkanRenderBackend.h.
* @brief The VulkanRenderBackend Class Definitions.
* @author Spices.
*/

#pragma once
#include "Core/Core.h"
#include "VulkanUtils.h"

#include "Render/FrameInfo.h"
#include "Core/Event/WindowEvent.h"
#include "Core/Event/SlateEvent.h"
#include "Core/Event/WorldEvent.h"

#include "VulkanWindows.h"
#include "VulkanInstance.h"
#include "VulkanDevice.h"
#include "VulkanCommandBuffer.h"
#include "VulkanRenderPass.h"
#include "VulkanSwapChain.h"
#include "VulkanDescriptor.h"
#include "VulkanDebugUtils.h"
#include "VulkanCmdThreadPool.h"

namespace Spices {

	/**
	* @brief Forward Declear.
	*/
	class RendererResourcePool;

	/**
	* @brief This class defines the renderbackend behaver of Vulkan.
	*/
	class VulkanRenderBackend
	{
	public:

		/**
		* @brief Constructor Function.
		*/
		VulkanRenderBackend();

		/**
		* @brief Destructor Function.
		*/
		virtual ~VulkanRenderBackend();

		/**
		* @brief Copy Constructor Function.
		* @note This Class not allowed copy behaver.
		*/
		VulkanRenderBackend(const VulkanRenderBackend&) = delete;

		/**
		* @brief Copy Assignment Operation.
		* @note This Class not allowed copy behaver.
		*/
		VulkanRenderBackend& operator=(const VulkanRenderBackend&) = delete;

		/**
		* @brief Determin whether window is closed.
		* @return Returns True if closed.
		*/
		bool isWindowClosed() { return glfwWindowShouldClose(m_VulkanState.m_Windows); }

		/**
		* @brief Wait for sync.
		* Used in render stage.
		*/
		static void WaitIdle() { vkDeviceWaitIdle(m_VulkanState.m_Device); }

		/**
		* @brief Start record a new frame with vulkan render backend.
		* @param[in] frameInfo FrameInfo.
		*/
		void BeginFrame(FrameInfo& frameInfo);

		/**
		* @brief End record the frame with vulkan render backend.
		* @param[in] frameInfo FrameInfo.
		*/
		void EndFrame(FrameInfo& frameInfo);

		/**
		* @brief Draw World.
		* @param[in] ts TimeStep.
		* @param[in] frameInfo FrameInfo.
		*/
		void RenderFrame(TimeStep& ts, FrameInfo& frameInfo);

		/**
		* @brief This function is called on global event function is called.
		* @param[in] event Event.
		*/
		void OnEvent(Event& event);

		/**
		* @brief Get VulkanState in use.
		* @return Returns the VulkanState in use.
		*/
		inline static VulkanState& GetState() { return m_VulkanState; }

		/**
		* @brief Get DescriptorPool in use.
		* @return Returns the shared pointer of VulkanDescriptorPool in use.
		*/
		inline static std::shared_ptr<VulkanDescriptorPool> GetDescriptorPool() { return m_VulkanDescriptorPool; }

		/**
		* @brief Get RendererResourcePool in use.
		* @return Returns the shared pointer of RendererResourcePool in use.
		*/
		inline static std::shared_ptr<RendererResourcePool> GetRendererResourcePool() { return m_RendererResourcePool; }

	private:

		/**
		* @brief This function is called on window is resized over.
		* @param[in] event WindowResizeOverEvent.
		* @return Returns true if need block event.
		* @attention: Minimize Window registy OnWindowResizeOver, OnSlateResize will not be registy.
		*/
		bool OnWindowResizeOver(WindowResizeOverEvent& event);

		/**
		* @brief This function is called on viewport is resized.
		* @param[in] event SlateResizeEvent.
		* @return Returns true if need block event.
		*/
		bool OnSlateResize(SlateResizeEvent& event);

		/**
		* @brief This function is called on worldmarkqueryer tick.
		* @param[in] event WorldEvent.
		* @return Returns true if need block event.
		*/
		bool OnMeshAddedWorldEvent(WorldEvent& event);

		/**
		* @brief Called OnSlateResize.
		*/
		void RecreateSwapChain();

	private:

		/**
		* @brief The VulkanState in use.
		*/
		static VulkanState m_VulkanState;

		/**
		* @brief The VulkanDescriptorPool in use.
		*/
		static std::shared_ptr<VulkanDescriptorPool> m_VulkanDescriptorPool;

		/**
		* @brief The RendererResourcePool in use.
		*/
		static std::shared_ptr<RendererResourcePool> m_RendererResourcePool;
		
		/**
		* @brief VulkanWindows.
		*/
		std::unique_ptr<VulkanWindows> m_VulkanWindows;

		/**
		* @brief VulkanInstance.
		*/
		std::unique_ptr<VulkanInstance> m_VulkanInstance;

		/**
		* @brief VulkanDevice.
		*/
		std::shared_ptr<VulkanDevice> m_VulkanDevice;

		/**
		* @brief VulkanCommandPool.
		*/
		std::unique_ptr<VulkanCommandPool> m_VulkanCommandPool;

		/**
		* @brief VulkanCommandBuffer.
		*/
		std::unique_ptr<VulkanCommandBuffer> m_VulkanCommandBuffer;

		/**
		* @brief VulkanSwapChain.
		*/
		std::unique_ptr<VulkanSwapChain> m_VulkanSwapChain;

		/**
		* @brief ThreadPool of Submit Commands. 
		*/
		std::shared_ptr<VulkanCmdThreadPool> m_CmdThreadPool;
	};
}