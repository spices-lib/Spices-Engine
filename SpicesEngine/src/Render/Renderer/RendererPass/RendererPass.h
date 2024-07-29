/**
* @file RendererPass.h
* @brief The RendererPass Class Definitions.
* @author Spices.
*/

#pragma once
#include "Core/Core.h"
#include "RendererSubPass.h"
#include "Core/Container/linked_unordered_map.h"

namespace Spices {

	/**
	* @brief This class is a wrapper of VulkanRenderPass.
	* Used during Renderer.
	*/
	class RendererPass
	{
	public:

		/**
		* @brief Constructor Function.
		* @param[in] passName The name of rendererpass.
		* @param[in] vulkanDevice VulkanDevice.
		*/
		RendererPass(
			const std::string&            passName     , 
			std::shared_ptr<VulkanDevice> vulkanDevice
		) 
			: m_PassName(passName)
			, m_Device(vulkanDevice)
		{}

		virtual ~RendererPass();

		/**
		* @brief Get SubPasses held by this renderer pass.
		* @return Return the SubPasses held by this renderer pass.
		*/
		scl::linked_unordered_map<std::string, std::shared_ptr<RendererSubPass>>& GetSubPasses() { return m_SubPasses; }

		/**
		* @brief Add a subpass to this renderer pass.
		* @param[in] subPassName The name of subpass.
		* @param[in] index The index of subpass.
		* @return Return the subpass.
		*/
		std::shared_ptr<RendererSubPass> AddSubPass(const std::string& subPassName, uint32_t index);

		/**
		* @brief Add a attachment to this renderer pass.
		* @param[in] attachmentName The name of attachment.
		* @param[in] description The description of attachment.
		* @param[in] layers The layers of attachment.
		* @param[in] clearValue The clear value of attachment.
		* @return Return the index of attachment.
		*/
		uint32_t AddAttachment(
			const std::string&             attachmentName ,
			const VkAttachmentDescription& description    ,
			uint32_t                       layers         ,
			const VkClearValue&            clearValue
		);

		/**
		* @brief Add a attachment to this renderer pass.
		* @param[in] attachmentName The name of attachment.
		* @param[in] description The description of attachment.
		* @param[in] clearValue The clear value of attachment.
		* @param[in] layers The layers of attachment.
		* @param[in] view The view of attachment.
		* @return Return the index of attachment.
		*/
		uint32_t AddAttachment(
			const std::string&             attachmentName  , 
			const VkAttachmentDescription& description     ,
			const VkClearValue&            clearValue      ,
			uint32_t                       layers          ,
			const VkImageView&             view
		);

		/**
		* @brief Build RenderPass.
		*/
		void BuildRendererPass();

		/**
		* @brief Get VkRenderPass.
		* @return Return VkRenderPass.
		*/
		VkRenderPass& Get() const { return m_RenderPass->Get(); }

		/**
		* @brief Get Renderer pass name.
		* @return Return Renderer pass name.
		*/
		std::string& GetName() { return m_PassName; }

		/**
		* @brief Get VkFramebuffer by index.
		* @param[in] index The index of framebuffer.
		* @return Return VkFramebuffer.
		*/
		VkFramebuffer& GetFramebuffer(uint32_t index) const { return m_RenderPass->GetFramebuffer(index); }

		/**
		* @brief Determine whether the RendererPass is using swapchain image.
		* @return Returns true if the RendererPass is using swapchain image.
		*/
		bool IsUseSwapChain() const { return m_IsSwapChainImageInUse; }

		/**
		* @brief Get ClearValues.
		* @return Return ClearValues.
		*/
		std::vector<VkClearValue>& GetClearValues() { return m_ClearValues; }

	private:

		/**
		* @brief RenderPass's name.
		*/
		std::string m_PassName;

		/**
		* @brief SubPass this RenderPass handled.
		*/
		scl::linked_unordered_map<std::string, std::shared_ptr<RendererSubPass>> m_SubPasses;
		
		/**
		* @brief VulkanRenderPass this RenderPass handled.
		*/
		std::unique_ptr<VulkanRenderPass> m_RenderPass;

		/**
		* @brief VkAttachmentDescription this RenderPass handled.
		*/
		scl::linked_unordered_map<std::string, VkAttachmentDescription> m_AttachmentDescriptions;

		/**
		* @brief VkClearValue this RenderPass handled.
		*/
		std::vector<VkClearValue> m_ClearValues;
		
		/**
		* @brief VkImageView this RenderPass handled.
		*/
		std::vector<VkImageView> m_ImageViews;

		/**
		* @brief VulkanDevice.
		*/
		std::shared_ptr<VulkanDevice> m_Device;

		/**
		* @brief Max Attachment layer.
		*/
		uint32_t m_MaxLayers = 1;

		/**
		* @brief True if the RendererPass is using swapchain image.
		*/
		bool m_IsSwapChainImageInUse = false;
	};
}