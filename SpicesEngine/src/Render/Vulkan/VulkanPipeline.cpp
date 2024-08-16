/**
* @file VulkanPipeline.cpp.
* @brief The VulkanPipeline Class Implementation.
* @author Spices.
*/

#include "Pchheader.h"
#include "VulkanPipeline.h"
#include "Resources/Mesh/Mesh.h"

namespace Spices {

	VulkanPipeline::VulkanPipeline(
		VulkanState&               vulkanState    , 
		const std::string&         pipelineName   ,
		const ShaderMap&           shaders       ,
		const PipelineConfigInfo&  config
	)
		: VulkanObject(vulkanState)
	{
		SPICES_PROFILE_ZONE;

		/**
		* @brief Create Pipeline.
		*/
		VulkanPipeline::CreateGraphicsPipeline(pipelineName, shaders, config);
	}

	VulkanPipeline::~VulkanPipeline()
	{
		SPICES_PROFILE_ZONE;

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
		SPICES_PROFILE_ZONE;

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
		configInfo.viewportInfo.pViewports                   = &configInfo.viewport;
		configInfo.viewportInfo.scissorCount                 = 1;
		configInfo.viewportInfo.pScissors                    = &configInfo.scissor;

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
		configInfo.depthStencilInfo.depthCompareOp           = VK_COMPARE_OP_GREATER;  /* @brief Though we reverse z depth, allow greater value pass. */
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

	void VulkanPipeline::CreateGraphicsPipeline(
		const std::string&         pipelineName  ,
		const ShaderMap&           shaders       ,
		const PipelineConfigInfo&  config
	)
	{
		SPICES_PROFILE_ZONE;

		/**
		* @brief Receive PipelineLayout from parameter.
		*/
		m_PipelineLayout = config.pipelineLayout;

		/**
		* @brief Create the VulkanShaderModule.
		*/
		std::vector<std::unique_ptr<VulkanShaderModule>> shaderModules;
		for (auto& pair : shaders)
		{
			if (pair.first == "rchit") continue;

			for (size_t i = 0; i < pair.second.size(); i++)
			{
				shaderModules.push_back(std::make_unique<VulkanShaderModule>(m_VulkanState, pair.second[i], pair.first));
			}
		}

		/**
		* @brief Instance VkPipelineShaderStageCreateInfo.
		*/
		std::vector<VkPipelineShaderStageCreateInfo> shaderStages;
		for (size_t i = 0; i < shaderModules.size(); i++)
		{
			shaderStages.push_back(shaderModules[i]->GetShaderStageCreateInfo());
		}
		
		auto& bindingDescriptions    = config.bindingDescriptions;
		auto& attributeDescriptions = config.attributeDescriptions;

		/**
		* @brief Instance a VkPipelineVertexInputStateCreateInfo.
		*/
		VkPipelineVertexInputStateCreateInfo              vertexInputInfo{};
		vertexInputInfo.sType                           = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
		vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size());
		vertexInputInfo.vertexBindingDescriptionCount   = static_cast<uint32_t>(bindingDescriptions.size());
		vertexInputInfo.pVertexAttributeDescriptions    = attributeDescriptions.data();
		vertexInputInfo.pVertexBindingDescriptions      = bindingDescriptions.data();

		/**
		* @brief Instance a VkGraphicsPipelineCreateInfo.
		*/
		VkGraphicsPipelineCreateInfo                      pipelineInfo{};
		pipelineInfo.sType                              = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
		pipelineInfo.stageCount                         = static_cast<uint32_t>(shaderStages.size());
		pipelineInfo.pStages                            = shaderStages.data();
		pipelineInfo.pVertexInputState                  = &vertexInputInfo;
		pipelineInfo.pInputAssemblyState                = &config.inputAssemblyInfo;
		pipelineInfo.pViewportState                     = &config.viewportInfo;
		pipelineInfo.pRasterizationState                = &config.rasterizationInfo;
		pipelineInfo.pMultisampleState                  = &config.multisampleInfo;
		pipelineInfo.pColorBlendState                   = &config.colorBlendInfo;
		pipelineInfo.pDepthStencilState                 = &config.depthStencilInfo;
		pipelineInfo.pDynamicState                      = &config.dynamicStateInfo;

		pipelineInfo.layout                             = m_PipelineLayout;
		pipelineInfo.renderPass                         = config.renderPass;
		pipelineInfo.subpass                            = config.subpass;

		pipelineInfo.basePipelineIndex                  = -1;
		pipelineInfo.basePipelineHandle                 = VK_NULL_HANDLE;

		/**
		* @brief Create Pipeline.
		*/
		VK_CHECK(vkCreateGraphicsPipelines(m_VulkanState.m_Device, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &m_Pipeline));
		VulkanDebugUtils::SetObjectName(VK_OBJECT_TYPE_PIPELINE, (uint64_t)m_Pipeline, m_VulkanState.m_Device, pipelineName);
	}

	VulkanRayTracingPipeline::VulkanRayTracingPipeline(
		VulkanState&               vulkanState  ,
		const std::string&         pipelineName ,
		const ShaderMap&           shaders      ,
		const PipelineConfigInfo&  config
	)
		: VulkanPipeline(vulkanState)
	{
		SPICES_PROFILE_ZONE;

		/**
		* @brief Create RT Pipeline.
		*/
		VulkanRayTracingPipeline::CreateGraphicsPipeline(pipelineName, shaders, config);
	}

	void VulkanRayTracingPipeline::CreateGraphicsPipeline(
		const std::string&         pipelineName , 
		const ShaderMap&           shaders      ,
		const PipelineConfigInfo&  config
	)
	{
		SPICES_PROFILE_ZONE;

		/**
		* @brief Receive PipelineLayout from parameter.
		*/
		m_PipelineLayout = config.pipelineLayout;

		/**
		* @brief Create the VulkanShaderModule.
		*/
		std::vector<std::unique_ptr<VulkanShaderModule>> shaderModules;
		for (auto& pair : shaders)
		{
			for(size_t i = 0; i < pair.second.size(); i++)
			{
				shaderModules.push_back(std::make_unique<VulkanShaderModule>(m_VulkanState, pair.second[i], pair.first));
			}
		}

		/**
		* @brief Instance VkPipelineShaderStageCreateInfo.
		*/
		std::vector<VkPipelineShaderStageCreateInfo> shaderStages;
		for (size_t i = 0; i < shaderModules.size(); i++)
		{
			shaderStages.push_back(shaderModules[i]->GetShaderStageCreateInfo());
		}

		/**
		* @brief Shader groups.
		*/
		VkRayTracingShaderGroupCreateInfoKHR  group{};
		group.sType                         = VK_STRUCTURE_TYPE_RAY_TRACING_SHADER_GROUP_CREATE_INFO_KHR;
		group.anyHitShader                  = VK_SHADER_UNUSED_KHR;
		group.closestHitShader              = VK_SHADER_UNUSED_KHR;
		group.generalShader                 = VK_SHADER_UNUSED_KHR;
		group.intersectionShader            = VK_SHADER_UNUSED_KHR;
		
		std::vector<VkRayTracingShaderGroupCreateInfoKHR> m_RTShaderGroups;

		for (auto& pair : shaders)
		{
			/**
			* @brief Raygen.
			*/
			if(pair.first == "rgen")
			{
				for(size_t i = 0; i < pair.second.size(); i++)
		     	{
		     		group.type                         = VK_RAY_TRACING_SHADER_GROUP_TYPE_GENERAL_KHR;
		     		group.generalShader                = static_cast<uint32_t>(m_RTShaderGroups.size());
		     	
		     		m_RTShaderGroups.push_back(group);
		     	}
		    }
			
			/**
			* @brief Miss.
			*/
			else if(pair.first == "rmiss")
			{
				for(size_t i = 0; i < pair.second.size(); i++)
				{
					group.type                        = VK_RAY_TRACING_SHADER_GROUP_TYPE_GENERAL_KHR;
					group.generalShader               = static_cast<uint32_t>(m_RTShaderGroups.size());
				
					m_RTShaderGroups.push_back(group);
				}
			}

			/**
			* @brief Closest hit.
			*/
			else if(pair.first == "rchit")
			{
				for(size_t i = 0; i < pair.second.size(); i++)
				{
					group.type                        = VK_RAY_TRACING_SHADER_GROUP_TYPE_TRIANGLES_HIT_GROUP_KHR;
					group.generalShader               = VK_SHADER_UNUSED_KHR;
					group.closestHitShader            = static_cast<uint32_t>(m_RTShaderGroups.size());
				
					m_RTShaderGroups.push_back(group);
				}
			}
			else
			{
				SPICES_CORE_ERROR("RayTracing Pipeline: Not Supported Sahder Stage from material.");
			}
		}
		
		/**
		* @brief Instance a VkGraphicsPipelineCreateInfo.
		*/
		VkRayTracingPipelineCreateInfoKHR                            rayPipelineInfo{};
		rayPipelineInfo.sType                                      = VK_STRUCTURE_TYPE_RAY_TRACING_PIPELINE_CREATE_INFO_KHR;
		rayPipelineInfo.stageCount                                 = static_cast<uint32_t>(shaderStages.size());
		rayPipelineInfo.pStages                                    = shaderStages.data();
		rayPipelineInfo.groupCount                                 = static_cast<uint32_t>(m_RTShaderGroups.size());
		rayPipelineInfo.pGroups                                    = m_RTShaderGroups.data();
		rayPipelineInfo.maxPipelineRayRecursionDepth               = 2;  /* @brief Ray depth */
		rayPipelineInfo.layout                                     = m_PipelineLayout;

		/**
		* @brief Create Pipeline.
		*/
		VK_CHECK(m_VulkanState.m_VkFunc.vkCreateRayTracingPipelinesKHR(m_VulkanState.m_Device, {}, {}, 1, &rayPipelineInfo, nullptr, &m_Pipeline))
		VulkanDebugUtils::SetObjectName(VK_OBJECT_TYPE_PIPELINE, (uint64_t)m_Pipeline, m_VulkanState.m_Device, pipelineName);
	}

	VulkanComputePipeline::VulkanComputePipeline(
		VulkanState&               vulkanState   , 
		const std::string&         pipelineName  , 
		const ShaderMap&           shaders       ,
		const PipelineConfigInfo&  config
	)
		:VulkanPipeline(vulkanState)
	{
		SPICES_PROFILE_ZONE;

		VulkanComputePipeline::CreateGraphicsPipeline(pipelineName, shaders, config);
	}

	void VulkanComputePipeline::CreateGraphicsPipeline(
		const std::string&         pipelineName  , 
		const ShaderMap&           shaders       ,
		const PipelineConfigInfo&  config
	)
	{
		SPICES_PROFILE_ZONE;

		/**
		* @brief Receive PipelineLayout from parameter.
		*/
		m_PipelineLayout = config.pipelineLayout;

		/**
		* @brief Create the VulkanShaderModule.
		*/
		std::vector<std::unique_ptr<VulkanShaderModule>> shaderModules;
		for (auto& pair : shaders)
		{
			if (pair.first == "comp")
			{
				shaderModules.push_back(std::make_unique<VulkanShaderModule>(m_VulkanState, pair.second[0], pair.first));
				break;
			}
		}

		/**
		* @brief Instance VkPipelineShaderStageCreateInfo.
		*/
		std::vector<VkPipelineShaderStageCreateInfo> shaderStages;
		for (size_t i = 0; i < shaderModules.size(); i++)
		{
			shaderStages.push_back(shaderModules[i]->GetShaderStageCreateInfo());
		}
		
		/**
		* @brief Instance VkComputePipelineCreateInfo.
		* @note one shader stage per compute pipeline.
		*/
		VkComputePipelineCreateInfo     pipelineInfo {};
		pipelineInfo.sType            = VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_CREATE_INFO;
		pipelineInfo.layout           = m_PipelineLayout;
		pipelineInfo.stage            = shaderStages[0];

		/**
		* @brief Create Pipeline.
		*/
		VK_CHECK(vkCreateComputePipelines(m_VulkanState.m_Device, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &m_Pipeline));
		VulkanDebugUtils::SetObjectName(VK_OBJECT_TYPE_PIPELINE, (uint64_t)m_Pipeline, m_VulkanState.m_Device, pipelineName);
	}

	VulkanMeshPipeline::VulkanMeshPipeline(
		VulkanState&               vulkanState   ,
		const std::string&         pipelineName  ,
		const ShaderMap&           shaders,
		const PipelineConfigInfo&  config
	)
		:VulkanPipeline(vulkanState)
	{
		SPICES_PROFILE_ZONE;

		VulkanMeshPipeline::CreateGraphicsPipeline(pipelineName, shaders, config);
	}

	void VulkanMeshPipeline::CreateGraphicsPipeline(
		const std::string&         pipelineName  ,
		const ShaderMap&           shaders       ,
		const PipelineConfigInfo&  config
	)
	{
		SPICES_PROFILE_ZONE;

		/**
		* @brief Receive PipelineLayout from parameter.
		*/
		m_PipelineLayout = config.pipelineLayout;

		/**
		* @brief Create the VulkanShaderModule.
		*/
		std::vector<std::unique_ptr<VulkanShaderModule>> shaderModules;
		for (auto& pair : shaders)
		{
			if (pair.first == "rchit") continue;

			for (size_t i = 0; i < pair.second.size(); i++)
			{
				shaderModules.push_back(std::make_unique<VulkanShaderModule>(m_VulkanState, pair.second[i], pair.first));
			}
		}

		/**
		* @brief Instance VkPipelineShaderStageCreateInfo.
		*/
		std::vector<VkPipelineShaderStageCreateInfo> shaderStages;
		for (size_t i = 0; i < shaderModules.size(); i++)
		{
			shaderStages.push_back(shaderModules[i]->GetShaderStageCreateInfo());
		}

		/**
		* @brief Instance a VkGraphicsPipelineCreateInfo.
		*/
		VkGraphicsPipelineCreateInfo                      pipelineInfo{};
		pipelineInfo.sType                              = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
		pipelineInfo.stageCount                         = static_cast<uint32_t>(shaderStages.size());
		pipelineInfo.pStages                            = shaderStages.data();
		pipelineInfo.pVertexInputState                  = nullptr;
		pipelineInfo.pInputAssemblyState                = nullptr;
		pipelineInfo.pViewportState                     = &config.viewportInfo;
		pipelineInfo.pRasterizationState                = &config.rasterizationInfo;
		pipelineInfo.pMultisampleState                  = &config.multisampleInfo;
		pipelineInfo.pColorBlendState                   = &config.colorBlendInfo;
		pipelineInfo.pDepthStencilState                 = &config.depthStencilInfo;
		pipelineInfo.pDynamicState                      = &config.dynamicStateInfo;

		pipelineInfo.layout                             = m_PipelineLayout;
		pipelineInfo.renderPass                         = config.renderPass;
		pipelineInfo.subpass                            = config.subpass;

		pipelineInfo.basePipelineIndex                  = -1;
		pipelineInfo.basePipelineHandle                 = VK_NULL_HANDLE;

		/**
		* @brief Create Pipeline.
		*/
		VK_CHECK(vkCreateGraphicsPipelines(m_VulkanState.m_Device, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &m_Pipeline))
		VulkanDebugUtils::SetObjectName(VK_OBJECT_TYPE_PIPELINE, (uint64_t)m_Pipeline, m_VulkanState.m_Device, pipelineName);
	}

	VulkanIndirectMeshPipelineNV::VulkanIndirectMeshPipelineNV(
		VulkanState&               vulkanState  , 
		const std::string&         pipelineName ,
		const ShaderMap&           shaders      ,
		const PipelineConfigInfo&  config
	)
		:VulkanPipeline(vulkanState)
	{
		SPICES_PROFILE_ZONE;

		VulkanIndirectMeshPipelineNV::CreateGraphicsPipeline(pipelineName, shaders, config);
	}

	void VulkanIndirectMeshPipelineNV::CreateGraphicsPipeline(
		const std::string&         pipelineName , 
		const ShaderMap&           shaders      ,
		const PipelineConfigInfo&  config
	)
	{
		SPICES_PROFILE_ZONE;

		/**
		* @brief Receive PipelineLayout from parameter.
		*/
		m_PipelineLayout = config.pipelineLayout;

		/**
		* @brief Create the VulkanShaderModule.
		*/
		std::vector<std::unique_ptr<VulkanShaderModule>> shaderModules;
		for (auto& pair : shaders)
		{
			for(size_t i = 0; i < pair.second.size(); i++)
			{
				shaderModules.push_back(std::make_unique<VulkanShaderModule>(m_VulkanState, pair.second[i], pair.first));
			}
		}

		/**
		* @brief Instance VkPipelineShaderStageCreateInfo.
		*/
		std::vector<VkPipelineShaderStageCreateInfo> shaderStages;
		for (size_t i = 0; i < shaderModules.size(); i++)
		{
			shaderStages.push_back(shaderModules[i]->GetShaderStageCreateInfo());
		}

		/**
		* @brief Shader groups.
		*/
		VkGraphicsPipelineShaderGroupsCreateInfoNV group{};
		group.sType                         = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_SHADER_GROUPS_CREATE_INFO_NV;
		
		std::vector<VkGraphicsShaderGroupCreateInfoNV> m_ShaderGroups;
		
		group.groupCount                    = m_ShaderGroups.size();
		group.pGroups                       = m_ShaderGroups.data();

		/**
		* @brief Instance a VkGraphicsPipelineCreateInfo.
		*/
		VkGraphicsPipelineCreateInfo                      pipelineInfo{};
		pipelineInfo.sType                              = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
		pipelineInfo.flags                              = VK_PIPELINE_CREATE_INDIRECT_BINDABLE_BIT_NV;
		pipelineInfo.stageCount                         = static_cast<uint32_t>(shaderStages.size());
		pipelineInfo.pStages                            = shaderStages.data();
		pipelineInfo.pVertexInputState                  = nullptr;
		pipelineInfo.pInputAssemblyState                = nullptr;
		pipelineInfo.pViewportState                     = &config.viewportInfo;
		pipelineInfo.pRasterizationState                = &config.rasterizationInfo;
		pipelineInfo.pMultisampleState                  = &config.multisampleInfo;
		pipelineInfo.pColorBlendState                   = &config.colorBlendInfo;
		pipelineInfo.pDepthStencilState                 = &config.depthStencilInfo;
		pipelineInfo.pDynamicState                      = &config.dynamicStateInfo;

		pipelineInfo.layout                             = m_PipelineLayout;
		pipelineInfo.renderPass                         = config.renderPass;
		pipelineInfo.subpass                            = config.subpass;

		pipelineInfo.basePipelineIndex                  = -1;
		pipelineInfo.basePipelineHandle                 = VK_NULL_HANDLE;

		pipelineInfo.pNext                              = &group;

		/**
		* @brief Create Pipeline.
		*/
		VK_CHECK(vkCreateGraphicsPipelines(m_VulkanState.m_Device, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &m_Pipeline))
		VulkanDebugUtils::SetObjectName(VK_OBJECT_TYPE_PIPELINE, (uint64_t)m_Pipeline, m_VulkanState.m_Device, pipelineName);
	}
}
