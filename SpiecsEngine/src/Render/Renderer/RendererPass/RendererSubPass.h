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
		RendererSubPass(const std::string& subPassName, uint32_t index) : m_SubpassName(subPassName), m_Index(index) {};
		virtual ~RendererSubPass() {};

		void AddColorAttachmentReference( 
			const VkAttachmentReference&               attachmentReference,
			const VkPipelineColorBlendAttachmentState& colorBlend
		);

		void AdDepthAttachmentReference(
			const VkAttachmentReference&               attachmentReference
		);

		void AddInputAttachmentReference(
			const VkAttachmentReference&               attachmentReference
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
		uint32_t GetIndex() { return m_Index; };
		inline std::vector<VkPipelineColorBlendAttachmentState>& GetColorBlend() { return m_ColorBlends; };

	private:

		std::string m_SubpassName;
		uint32_t m_Index;
		VkSubpassDescription m_SubPassDescriptions{};
		VkSubpassDependency m_SubPassDependency{};

		std::vector<VkAttachmentReference> m_ColorAttachmentReference;
		std::vector<VkAttachmentReference> m_DepthAttachmentReference;
		std::vector<VkAttachmentReference> m_InputAttachmentReference;

		std::vector<VkPipelineColorBlendAttachmentState> m_ColorBlends;

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