#pragma once
#include "Core/Core.h"
#include "RendererSubPass.h"
#include "Core/Container/linked_unordered_map.h"

namespace Spiecs
{
	class RendererPass
	{
	public:
		RendererPass(
			const std::string& passName, 
			std::shared_ptr<VulkanDevice> vulkanDevice
		) 
			: m_PassName(passName)
			, m_Device(vulkanDevice)
		{};

		virtual ~RendererPass();

		inline scl::linked_unordered_map<std::string, std::shared_ptr<RendererSubPass>>& GetSubPasses() { return m_SubPasses; };
		std::shared_ptr<RendererSubPass> AddSubPass(const std::string& subPassName, uint32_t index);

		uint32_t AddAttachment(
			const std::string&             attachmnetName ,
			const VkAttachmentDescription& description    ,
			const VkClearValue&            clearValue
		);

		uint32_t AddAttachment(
			const std::string&             attachmnetName  , 
			const VkAttachmentDescription& description     ,
			const VkClearValue&            clearValue      ,
			VkImageView&                   view
		);

		void BuildRendererPass();

		VkRenderPass& Get() { return m_RenderPass->Get(); };
		inline std::string& GetName() { return m_PassName; };
		VkFramebuffer& GetFramebuffer(uint32_t index) { return m_RenderPass->GetFramebuffer(index); };
		inline const bool IsUseSwapChain() const { return m_IsSwapChainImageInUse; };

		inline std::vector<VkClearValue>& GetClearValues() { return m_ClearValues; };

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

		scl::linked_unordered_map<std::string, VkAttachmentDescription> m_AttachmentDescriptions;

		std::vector<VkClearValue> m_ClearValues;
		

		std::vector<VkImageView> m_ImageViews;

		std::shared_ptr<VulkanDevice> m_Device;

		bool m_IsSwapChainImageInUse = false;
	};
}