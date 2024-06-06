/**
* @file VulkanPipeline.cpp.
* @brief The VulkanPipeline Class Implementation.
* @author Spiecs.
*/

#include "Pchheader.h"
#include "VulkanPipeline.h"
#include "Resources/Mesh/Mesh.h"

namespace Spiecs {

	VulkanPipeline::VulkanPipeline(
		VulkanState&              vulkanState    , 
		const std::string&        pipelineName   ,
		const std::string&        vertShaderName ,
		const std::string&        fragShaderName ,
		const PipelineConfigInfo& config
	)
		: VulkanObject(vulkanState)
	{
		SPIECS_PROFILE_ZONE;

		/**
		* @brief Create Pipeline.
		*/
		CreateGraphicsPipeline(pipelineName, vertShaderName, fragShaderName, config);
	}

	VulkanPipeline::~VulkanPipeline()
	{
		SPIECS_PROFILE_ZONE;

		/**
		* @brief Destroy PipelineLayout.
		*/
		vkDestroyPipelineLayout(m_VulkanState.m_Device, m_PipelineLayout, nullptr);

		/**
		* @brief Destroy Pipeline.
		*/
		vkDestroyPipeline(m_VulkanState.m_Device, m_Pipeline, nullptr);
	}

	void VulkanPipeline::DefaultPipelineConfigInfo(PipelineConfigInfo& configInfo)
	{
		SPIECS_PROFILE_ZONE;

		/**
		* @brief VkPipelineInputAssemblyStateCreateInfo.
		*/
		configInfo.inputAssemblyInfo                         = VkPipelineInputAssemblyStateCreateInfo{};
		configInfo.inputAssemblyInfo.sType                   = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
		configInfo.inputAssemblyInfo.topology                = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
		configInfo.inputAssemblyInfo.primitiveRestartEnable  = VK_FALSE;

		/**
		* @brief VkPipelineViewportStateCreateInfo.
		*/
		configInfo.viewportInfo.sType                        = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
		configInfo.viewportInfo.viewportCount                = 1;
		configInfo.viewportInfo.pViewports                   = nullptr;
		configInfo.viewportInfo.scissorCount                 = 1;
		configInfo.viewportInfo.pScissors                    = nullptr;

		/**
		* @brief VkPipelineRasterizationStateCreateInfo.
		*/
		configInfo.rasterizationInfo                         = VkPipelineRasterizationStateCreateInfo{};
		configInfo.rasterizationInfo.sType                   = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
		configInfo.rasterizationInfo.depthClampEnable        = VK_FALSE;
		configInfo.rasterizationInfo.rasterizerDiscardEnable = VK_FALSE;
		configInfo.rasterizationInfo.polygonMode             = VK_POLYGON_MODE_FILL;
		configInfo.rasterizationInfo.lineWidth               = 1.0f;
		configInfo.rasterizationInfo.cullMode                = VK_CULL_MODE_BACK_BIT;
		configInfo.rasterizationInfo.frontFace               = VK_FRONT_FACE_COUNTER_CLOCKWISE; // negative clockwise
		configInfo.rasterizationInfo.depthBiasEnable         = VK_FALSE;
		configInfo.rasterizationInfo.depthBiasConstantFactor = 0.0f;
		configInfo.rasterizationInfo.depthBiasClamp          = 0.0f;
		configInfo.rasterizationInfo.depthBiasSlopeFactor    = 0.0f;

		/**
		* @brief VkPipelineMultisampleStateCreateInfo.
		*/
		configInfo.multisampleInfo                           = VkPipelineMultisampleStateCreateInfo{};
		configInfo.multisampleInfo.sType                     = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
		configInfo.multisampleInfo.sampleShadingEnable       = VK_FALSE;
		configInfo.multisampleInfo.rasterizationSamples      = VK_SAMPLE_COUNT_1_BIT; // TODO: Configurable, Disable MASS : VK_SAMPLE_COUNT_1_BIT, Enable MASS: VK_SAMPLE_COUNT_8_BIT.
		configInfo.multisampleInfo.minSampleShading          = 1.0f;
		configInfo.multisampleInfo.pSampleMask               = nullptr;
		configInfo.multisampleInfo.alphaToCoverageEnable     = VK_FALSE;
		configInfo.multisampleInfo.alphaToOneEnable          = VK_FALSE;

		/**
		* @brief VkPipelineColorBlendStateCreateInfo.
		*/
		configInfo.colorBlendInfo                            = VkPipelineColorBlendStateCreateInfo{};
		configInfo.colorBlendInfo.sType                      = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
		configInfo.colorBlendInfo.logicOpEnable              = VK_FALSE;
		configInfo.colorBlendInfo.logicOp                    = VK_LOGIC_OP_COPY;

		configInfo.colorBlendInfo.attachmentCount            = 0;
		configInfo.colorBlendInfo.pAttachments               = nullptr;
		configInfo.colorBlendInfo.blendConstants[0]          = 0.0f;
		configInfo.colorBlendInfo.blendConstants[1]          = 0.0f;
		configInfo.colorBlendInfo.blendConstants[2]          = 0.0f;
		configInfo.colorBlendInfo.blendConstants[3]          = 0.0f;

		/**
		* @brief VkPipelineDepthStencilStateCreateInfo.
		*/
		configInfo.depthStencilInfo                          = VkPipelineDepthStencilStateCreateInfo{};
		configInfo.depthStencilInfo.sType                    = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
		configInfo.depthStencilInfo.depthTestEnable          = VK_TRUE;
		configInfo.depthStencilInfo.depthWriteEnable         = VK_TRUE;
		configInfo.depthStencilInfo.depthCompareOp           = VK_COMPARE_OP_LESS;
		configInfo.depthStencilInfo.depthBoundsTestEnable    = VK_FALSE;
		configInfo.depthStencilInfo.minDepthBounds           = 0.0f;
		configInfo.depthStencilInfo.maxDepthBounds           = 1.0f;
		configInfo.depthStencilInfo.stencilTestEnable        = VK_FALSE;
		configInfo.depthStencilInfo.front                    = {};
		configInfo.depthStencilInfo.back                     = {};

		/**
		* @brief VkDynamicState.
		*/
		configInfo.dynamicStateEnables                       = { VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR };
		configInfo.dynamicStateInfo.sType                    = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
		configInfo.dynamicStateInfo.pDynamicStates           = configInfo.dynamicStateEnables.data();
		configInfo.dynamicStateInfo.dynamicStateCount        = static_cast<uint32_t>(configInfo.dynamicStateEnables.size());
		configInfo.dynamicStateInfo.flags                    = 0;

		/**
		* @brief VkVertexInputBindingDescription.
		*/
		configInfo.bindingDescriptions                       = Vertex::GetBindingDescriptions();

		/**
		* @brief VkVertexInputAttributeDescription.
		*/
		configInfo.attributeDescriptions                     = Vertex::GetAttributeDescriptions();
	}

	void VulkanPipeline::Bind(uint32_t frameIndex)
	{
		SPIECS_PROFILE_ZONE;

		/**
		* @brief Bind Pipeline.
		*/
		vkCmdBindPipeline(m_VulkanState.m_CommandBuffer[frameIndex], VK_PIPELINE_BIND_POINT_GRAPHICS, m_Pipeline);
	}

	void VulkanPipeline::CreateGraphicsPipeline(
		const std::string&        pipelineName    ,
		const std::string&        vertShaderName  ,
		const std::string&        fragShaderName  ,
		const PipelineConfigInfo& config
	)
	{
		SPIECS_PROFILE_ZONE;

		/**
		* @brief Receive PipelineLayout from parameter.
		*/
		m_PipelineLayout = config.pipelineLayout;

		/**
		* @brief Create the VulkanShaderModule.
		*/
		m_VertShaderModule = std::make_unique<VulkanShaderModule>(m_VulkanState, vertShaderName, "vert");
		m_FragShaderModule = std::make_unique<VulkanShaderModule>(m_VulkanState, fragShaderName, "frag");

		/**
		* @brief Instance VkPipelineShaderStageCreateInfo.
		*/
		VkPipelineShaderStageCreateInfo shaderStages[2];
		shaderStages[0].sType                = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		shaderStages[0].stage                = VK_SHADER_STAGE_VERTEX_BIT;
		shaderStages[0].module               = m_VertShaderModule->Get();
		shaderStages[0].pName                = "main";
		shaderStages[0].flags                = 0;
		shaderStages[0].pNext                = nullptr;
		shaderStages[0].pSpecializationInfo  = nullptr;

		shaderStages[1].sType                = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		shaderStages[1].stage                = VK_SHADER_STAGE_FRAGMENT_BIT;
		shaderStages[1].module               = m_FragShaderModule->Get();
		shaderStages[1].pName                = "main";
		shaderStages[1].flags                = 0;
		shaderStages[1].pNext                = nullptr;
		shaderStages[1].pSpecializationInfo  = nullptr;
		
		auto& bindingDescriptions = config.bindingDescriptions;
		auto& attributeDescriptions = config.attributeDescriptions;

		/**
		* @brief Instance a VkPipelineVertexInputStateCreateInfo.
		*/
		VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
		vertexInputInfo.sType                           = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
		vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size());
		vertexInputInfo.vertexBindingDescriptionCount   = static_cast<uint32_t>(bindingDescriptions.size());
		vertexInputInfo.pVertexAttributeDescriptions    = attributeDescriptions.data();
		vertexInputInfo.pVertexBindingDescriptions      = bindingDescriptions.data();

		/**
		* @brief Instance a VkGraphicsPipelineCreateInfo.
		*/
		VkGraphicsPipelineCreateInfo pipelineInfo{};
		pipelineInfo.sType                              = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
		pipelineInfo.stageCount                         = 2;
		pipelineInfo.pStages                            = shaderStages;
		pipelineInfo.pVertexInputState                  = &vertexInputInfo;
		pipelineInfo.pInputAssemblyState                = &config.inputAssemblyInfo;
		pipelineInfo.pViewportState                     = &config.viewportInfo;
		pipelineInfo.pRasterizationState                = &config.rasterizationInfo;
		pipelineInfo.pMultisampleState                  = &config.multisampleInfo;
		pipelineInfo.pColorBlendState                   = &config.colorBlendInfo;
		pipelineInfo.pDepthStencilState                 = &config.depthStencilInfo;
		pipelineInfo.pDynamicState                      = &config.dynamicStateInfo;

		pipelineInfo.layout                             = config.pipelineLayout;
		pipelineInfo.renderPass                         = config.renderPass;
		pipelineInfo.subpass                            = config.subpass;

		pipelineInfo.basePipelineIndex                  = -1;
		pipelineInfo.basePipelineHandle                 = VK_NULL_HANDLE;

		/**
		* @brief Create Pipeline.
		*/
		VK_CHECK(vkCreateGraphicsPipelines(m_VulkanState.m_Device, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &m_Pipeline));
		VulkanDebugUtils::SetObjectName(VK_OBJECT_TYPE_PIPELINE, m_Pipeline, m_VulkanState.m_Device, pipelineName);
	}
}