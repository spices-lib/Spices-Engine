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

		virtual ~RendererPass() {};

		scl::linked_unordered_map<std::string, std::shared_ptr<RendererSubPass>>& GetSubPasses() { return m_SubPasses; };
		std::shared_ptr<RendererSubPass> AddSubPass(const std::string& subPassName);


		void AddAttachmentDescription(
			const std::string&             attachmnetName , 
			const VkAttachmentDescription& description    ,
			const VkClearValue&            clearValue
		);

		void BuildRendererPass();

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

		std::shared_ptr<VulkanDevice> m_Device;
	};
}