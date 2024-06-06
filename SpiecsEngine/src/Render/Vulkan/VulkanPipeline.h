/**
* @file VulkanPipeline.h.
* @brief The VulkanPipeline Class Definitions.
* @author Spiecs.
*/

#pragma once
#include "Core/Core.h"
#include "VulkanUtils.h"
#include "VulkanShaderModule.h"

namespace Spiecs {

	/**
	* @brief This struct included all infos usd to create a VkPipeline.
	*/
	struct PipelineConfigInfo 
	{
		/**
		* @brief Copy Constructor Function.
		* @note This Class not allowed copy behaver.
		*/
		PipelineConfigInfo() = default;

		/**
		* @brief Copy Constructor Function.
		* @note This Class not allowed copy behaver.
		*/
		PipelineConfigInfo(const PipelineConfigInfo&) = delete;

		/**
		* @brief Copy Assignment Operation.
		* @note This Class not allowed copy behaver.
		*/
		PipelineConfigInfo& operator=(const PipelineConfigInfo&) = delete;

		/**
		* @brief VertexInputBindingDescription, from Vertex.
		*/
		std::vector<VkVertexInputBindingDescription>   bindingDescriptions{};

		/**
		* @brief VertexInputAttributeDescription, from Vertex.
		*/
		std::vector<VkVertexInputAttributeDescription> attributeDescriptions{};

		/**
		* @brief VkPipelineViewportStateCreateInfo.
		*/
		VkPipelineViewportStateCreateInfo              viewportInfo;

		/**
		* @brief VkPipelineInputAssemblyStateCreateInfo.
		*/
		VkPipelineInputAssemblyStateCreateInfo         inputAssemblyInfo;

		/**
		* @brief VkPipelineRasterizationStateCreateInfo.
		*/
		VkPipelineRasterizationStateCreateInfo         rasterizationInfo;

		/**
		* @brief VkPipelineMultisampleStateCreateInfo.
		*/
		VkPipelineMultisampleStateCreateInfo           multisampleInfo;

		/**
		* @brief VkPipelineColorBlendStateCreateInfo.
		*/
		VkPipelineColorBlendStateCreateInfo            colorBlendInfo;

		/**
		* @brief VkPipelineDepthStencilStateCreateInfo.
		*/
		VkPipelineDepthStencilStateCreateInfo          depthStencilInfo;

		/**
		* @brief VkDynamicState.
		*/
		std::vector<VkDynamicState>                    dynamicStateEnables;

		/**
		* @brief VkPipelineDynamicStateCreateInfo.
		*/
		VkPipelineDynamicStateCreateInfo               dynamicStateInfo;

		/**
		* @brief VkPipelineLayout.
		*/
		VkPipelineLayout                               pipelineLayout = nullptr;

		/**
		* @brief VkRenderPass.
		*/
		VkRenderPass                                   renderPass = nullptr;

		/**
		* @brief SubPass num.
		*/
		uint32_t                                       subpass = 0;
	};

	/**
	* @brief This class is a wapper of VkPipelineLayout and VkPipeline.
	*/
	class VulkanPipeline : public VulkanObject
	{
	public:

		/**
		* @brief Constructor Function.
		* Create VkPipeline.
		* @param[in] vulkanState The global VulkanState.
		* @param[in] pipelineName The Pipeline name.
		* @param[in] vertShaderName The vert Shader name.
		* @param[in] fragShaderName The frag Shader name.
		* @param[in] config PipelineConfigInfo.
		*/
		VulkanPipeline(
			VulkanState&               vulkanState   ,
			const std::string&         pipelineName  ,
			const std::string&         vertShaderName,
			const std::string&         fragShaderName,
			const PipelineConfigInfo&  config
		);

		/**
		* @brief Destructor Function.
		*/
		virtual ~VulkanPipeline();

		/**
		* @brief Create a PipelineConfigInfo with default parameter.
		* @param[in out] configInfo PipelineConfigInfo.
		*/
		static void DefaultPipelineConfigInfo(PipelineConfigInfo& configInfo);

		/**
		* @brief Bind Pipeline with commandbuffer index.
		* @param[in] frameIndex The CommandBuffer Index.
		*/
		void Bind(uint32_t frameIndex);

		/**
		* @brief Get VkPipelineLayout.
		* @return Returns the VkPipelineLayout.
		*/
		inline VkPipelineLayout& GetPipelineLayout() { return m_PipelineLayout; };

	private:

		/**
		* @brief Create the VkPipeline.
		* @param[in] pipelineName The Pipeline name.
		* @param[in] vertShaderName The vert Shader name.
		* @param[in] fragShaderName The frag Shader name.
		* @param[in] config PipelineConfigInfo.
		*/
		void CreateGraphicsPipeline(
			const std::string&        pipelineName   ,
			const std::string&        vertShaderName , 
			const std::string&        fragShaderName , 
			const PipelineConfigInfo& config
		);

	private:

		/**
		* @brief The VkPipelineLayout.
		* @attention Create by Renderer and destroy by this.
		*/
		VkPipelineLayout m_PipelineLayout;

		/**
		* @brief The VkPipeline.
		*/
		VkPipeline m_Pipeline;

		/**
		* @brief The uniqie pointer of vert VulkanShaderModule.
		*/
		std::unique_ptr<VulkanShaderModule> m_VertShaderModule;

		/**
		* @brief The uniqie pointer of frag VulkanShaderModule.
		*/
		std::unique_ptr<VulkanShaderModule> m_FragShaderModule;
	};
}