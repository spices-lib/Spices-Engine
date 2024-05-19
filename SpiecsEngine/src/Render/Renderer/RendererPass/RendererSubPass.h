#pragma once
#include "Core/Core.h"

/******************************Vulkan Backend Header************************************************/
#include "Render/FrameInfo.h"
#include "Render/Vulkan/VulkanPipeline.h"
#include "Render/Vulkan/VulkanUtils.h"
#include "Render/Vulkan/VulkanBuffer.h"
#include "Render/Vulkan/VulkanImage.h"
#include "Render/Vulkan/VulkanDescriptor.h"
#include "Render/Vulkan/VulkanRenderPass.h"
/***************************************************************************************************/

namespace Spiecs
{
	class RendererSubPass
	{
	public:
		RendererSubPass(const std::string& subPassName) : m_SubpassName(subPassName) {};
		virtual ~RendererSubPass() {};

		void AddColorAttachmentReference(
			const std::string&           attachmentName      , 
			VkAttachmentReference&       attachmentReference
		);

		void AdDepthAttachmentReference(
			const std::string&           attachmentName      ,
			VkAttachmentReference&       attachmentReference
		);

		void AddInputAttachmentReference(
			const std::string&           attachmentName      ,
			VkAttachmentReference&       attachmentReference
		);

		void BuildSubPassDescription();

	private:

		std::string m_SubpassName;
		VkSubpassDescription m_SubPassDescriptions{};

		std::unordered_map<std::string, VkAttachmentReference> m_ColorAttachmentReference;
		std::unordered_map<std::string, VkAttachmentReference> m_DepthAttachmentReference;
		std::unordered_map<std::string, VkAttachmentReference> m_InputAttachmentReference;
	};
}