#pragma once
#include "Core/Core.h"
#include "VulkanUtils.h"
#include "VulkanShaderModule.h"

namespace Spiecs {

	struct PipelineConfigInfo {
		PipelineConfigInfo() = default;
		PipelineConfigInfo(const PipelineConfigInfo&) = delete;
		PipelineConfigInfo& operator=(const PipelineConfigInfo&) = delete;

		std::vector<VkVertexInputBindingDescription> bindingDescriptions{};
		std::vector<VkVertexInputAttributeDescription> attributeDescriptions{};
		VkPipelineViewportStateCreateInfo viewportInfo;
		VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo;
		VkPipelineRasterizationStateCreateInfo rasterizationInfo;
		VkPipelineMultisampleStateCreateInfo multisampleInfo;
		VkPipelineColorBlendStateCreateInfo colorBlendInfo;
		VkPipelineDepthStencilStateCreateInfo depthStencilInfo;
		std::vector<VkDynamicState> dynamicStateEnables;
		VkPipelineDynamicStateCreateInfo dynamicStateInfo;
		VkPipelineLayout pipelineLayout = nullptr;
		VkRenderPass renderPass = nullptr;
		uint32_t subpass = 0;
	};

	class VulkanPipeline : public VulkanObject
	{
	public:
		VulkanPipeline(
			VulkanState&               vulkanState  ,
			const std::string&         vertFilepath , 
			const std::string&         fragFilepath , 
			const PipelineConfigInfo&  config
		);

		virtual ~VulkanPipeline();

		static void DefaultPipelineConfigInfo(PipelineConfigInfo& configInfo);

		void Bind(uint32_t frameIndex);
		VkPipelineLayout& GetPipelineLayout() { return m_PipelineLayout; };

	private:
		void CreateGraphicsPipeline(const std::string& vertFilepath, const std::string& fragFilepath, const PipelineConfigInfo& config);

	private:

		VkPipelineLayout m_PipelineLayout;
		VkPipeline m_Pipeline;

		std::unique_ptr<VulkanShaderModule> m_VertShaderModule;
		std::unique_ptr<VulkanShaderModule> m_FragShaderModule;
	};
}