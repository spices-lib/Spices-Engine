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
			uint32_t                index,
			VkAttachmentReference&  attachmentReference
		);

		void AdDepthAttachmentReference(
			uint32_t                 index,
			VkAttachmentReference&   attachmentReference
		);

		void AddInputAttachmentReference(
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
		void AddPipeline(const std::string& name, std::shared_ptr<VulkanPipeline> pipeline);
		inline bool IsUsePushConstant() { return isUsePushConstant; };
		inline VkPushConstantRange& GetPushConstant() { return m_PushConstantRange; };
		inline std::unordered_map<std::string, std::shared_ptr<VulkanPipeline>>& GetPipelines() { return m_Pipelines; };
		inline std::unordered_map<Int2, std::unique_ptr<VulkanBuffer>>& GetBuffers() { return m_Buffers; };
		void SetBuffer(const Int2& i2, void* data);

	private:

		std::string m_SubpassName;
		VkSubpassDescription m_SubPassDescriptions{};
		VkSubpassDependency m_SubPassDependency{};

		std::vector<VkAttachmentReference> m_ColorAttachmentReference;
		std::vector<VkAttachmentReference> m_DepthAttachmentReference;
		std::vector<VkAttachmentReference> m_InputAttachmentReference;

		bool isUsePushConstant = false;
		VkPushConstantRange m_PushConstantRange{};
		

		std::unordered_map<Int2, std::unique_ptr<VulkanBuffer>> m_Buffers;

		std::unordered_map<std::string, std::shared_ptr<VulkanPipeline>> m_Pipelines;
	};

	template<typename T>
	inline void RendererSubPass::SetPushConstant(T fn)
	{
		isUsePushConstant = true;

		fn(m_PushConstantRange);
	}
}