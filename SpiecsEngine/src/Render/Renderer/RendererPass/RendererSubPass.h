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
		
		void BuildFirstSubPassDependency();
		void BuildSubPassDependency(uint32_t index);

		template<typename T>
		void SetPushConstant(T fn);

		VkSubpassDescription& GetDescription() { return m_SubPassDescriptions; };
		VkSubpassDependency& GetDependency() { return m_SubPassDependency; };
		const std::string& GetName() { return m_SubpassName; };

	private:

		std::string m_SubpassName;
		VkSubpassDescription m_SubPassDescriptions{};
		VkSubpassDependency m_SubPassDependency{};

		scl::linked_unordered_map<std::string, VkAttachmentReference> m_ColorAttachmentReference;
		scl::linked_unordered_map<std::string, VkAttachmentReference> m_DepthAttachmentReference;
		scl::linked_unordered_map<std::string, VkAttachmentReference> m_InputAttachmentReference;

		bool isUsePushConstant = false;
		VkPushConstantRange m_PushConstantRange{};
		

		std::unordered_map<Int2, std::unique_ptr<VulkanBuffer>> m_Buffers;
	};

	template<typename T>
	inline void RendererSubPass::SetPushConstant(T fn)
	{
		isUsePushConstant = true;

		fn(m_PushConstantRange);
	}
}