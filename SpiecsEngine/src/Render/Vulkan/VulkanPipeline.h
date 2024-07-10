/**
* @file VulkanPipeline.h.
* @brief The VulkanPipeline Class Definitions.
* @author Spiecs.
*/

#pragma once
#include "Core/Core.h"
#include "VulkanUtils.h"

namespace Spiecs {

	/**
	* @brief This struct included all infos usd to create a VkPipeline.
	*/
	struct PipelineConfigInfo 
	{
		/**
		* @brief Copy Constructor Function.
		* @note This Class not allowed copy behaves.
		*/
		PipelineConfigInfo() = default;

		/**
		* @brief Copy Constructor Function.
		* @note This Class not allowed copy behaves.
		*/
		PipelineConfigInfo(const PipelineConfigInfo&) = delete;

		/**
		* @brief Copy Assignment Operation.
		* @note This Class not allowed copy behaves.
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
		* @brief Viewport.
		*/
		VkViewport viewport{};

		/**
		* @brief Scissor.
		*/
		VkRect2D scissor{};

		/**
		* @brief VkPipelineViewportStateCreateInfo.
		*/
		VkPipelineViewportStateCreateInfo              viewportInfo{};

		/**
		* @brief VkPipelineInputAssemblyStateCreateInfo.
		*/
		VkPipelineInputAssemblyStateCreateInfo         inputAssemblyInfo{};

		/**
		* @brief VkPipelineRasterizationStateCreateInfo.
		*/
		VkPipelineRasterizationStateCreateInfo         rasterizationInfo{};

		/**
		* @brief VkPipelineMultisampleStateCreateInfo.
		*/
		VkPipelineMultisampleStateCreateInfo           multisampleInfo{};

		/**
		* @brief VkPipelineColorBlendStateCreateInfo.
		*/
		VkPipelineColorBlendStateCreateInfo            colorBlendInfo{};

		/**
		* @brief VkPipelineDepthStencilStateCreateInfo.
		*/
		VkPipelineDepthStencilStateCreateInfo          depthStencilInfo{};

		/**
		* @brief VkDynamicState.
		*/
		std::vector<VkDynamicState>                    dynamicStateEnables;

		/**
		* @brief VkPipelineDynamicStateCreateInfo.
		*/
		VkPipelineDynamicStateCreateInfo               dynamicStateInfo{};

		/**
		* @brief VkPipelineLayout.
		*/
		VkPipelineLayout                               pipelineLayout{};

		/**
		* @brief VkRenderPass.
		*/
		VkRenderPass                                   renderPass{};

		/**
		* @brief SubPass num.
		*/
		uint32_t                                       subpass = 0;
	};

	/**
	* @brief This class is a wrapper of VkPipelineLayout and VkPipeline.
	*/
	class VulkanPipeline : public VulkanObject
	{
	public:

		/**
		* @brief Constructor Function.
		* @param[in] vulkanState The global VulkanState.
		*/
		VulkanPipeline(VulkanState& vulkanState) : VulkanObject(vulkanState) {}

		/**
		* @brief Constructor Function.
		* Create VkPipeline.
		* @param[in] vulkanState The global VulkanState.
		* @param[in] pipelineName The Pipeline name.
		* @param[in] shaders The Shader stage name and path.
		* @param[in] config PipelineConfigInfo.
		*/
		VulkanPipeline(
			VulkanState&                                                      vulkanState   ,
			const std::string&                                                pipelineName  ,
			const std::unordered_map<std::string, std::vector<std::string>>&  shaders       ,
			const PipelineConfigInfo&                                         config
		);

		/**
		* @brief Destructor Function.
		*/
		virtual ~VulkanPipeline() override;

		/**
		* @brief Create a PipelineConfigInfo with default parameter.
		* @param[in,out] configInfo PipelineConfigInfo.
		*/
		static void DefaultPipelineConfigInfo(PipelineConfigInfo& configInfo);

		/**
		* @brief Bind Pipeline with commandbuffer index.
		* @param[in] frameIndex The CommandBuffer Index.
		* @param[in] bindPoint VkPipelineBindPoint.
		*/
		void Bind(uint32_t frameIndex, VkPipelineBindPoint bindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS) const;

		/**
		* @brief Get VkPipelineLayout.
		* @return Returns the VkPipelineLayout.
		*/
		VkPipelineLayout& GetPipelineLayout() { return m_PipelineLayout; }

		/**
		* @brief Get VkPipeline.
		* @return Returns the VkPipeline.
		*/
		VkPipeline& GetPipeline() { return m_Pipeline; }

	private:

		/**
		* @brief Create the VkPipeline.
		* @param[in] pipelineName The Pipeline name.
		* @param[in] shaders The Shader stage name and path.
		* @param[in] config PipelineConfigInfo.
		*/
		virtual void CreateGraphicsPipeline(
			const std::string&                                                pipelineName   ,
			const std::unordered_map<std::string, std::vector<std::string>>&  shaders       ,
			const PipelineConfigInfo&                                         config
		);

	protected:

		/**
		* @brief The VkPipelineLayout.
		* @attention Create by Renderer and destroy by this.
		*/
		VkPipelineLayout m_PipelineLayout{};

		/**
		* @brief The VkPipeline.
		*/
		VkPipeline m_Pipeline{};
	};

	/**
	* @brief This class is a wrapper of VkPipelineLayout and VkPipeline.
	*/
	class VulkanRayTracingPipeline : public VulkanPipeline
	{
	public:

		/**
		* @brief Constructor Function.
		* Create VkPipeline.
		* @param[in] vulkanState The global VulkanState.
		* @param[in] pipelineName The Pipeline name.
		* @param[in] shaders The Shader stage name and path.
		* @param[in] config PipelineConfigInfo.
		*/
		VulkanRayTracingPipeline(
			VulkanState&                                                      vulkanState  ,
			const std::string&                                                pipelineName ,
			const std::unordered_map<std::string, std::vector<std::string>>&  shaders       , 
			const PipelineConfigInfo&                                         config
		);

		/**
		* @brief Destructor Function.
		*/
		virtual ~VulkanRayTracingPipeline() override = default;

	private:

		/**
		* @brief Create the VkPipeline.
		* @param[in] pipelineName The Pipeline name.
		* @param[in] shaders The Shader stage name and path.
		* @param[in] config PipelineConfigInfo.
		*/
		virtual void CreateGraphicsPipeline(
			const std::string&                                                pipelineName ,
			const std::unordered_map<std::string, std::vector<std::string>>&  shaders       ,
			const PipelineConfigInfo&                                         config
		) override;

	private:

		std::vector<VkRayTracingShaderGroupCreateInfoKHR> m_RTShaderGroups;
		PFN_vkCreateRayTracingPipelinesKHR vkCreateRayTracingPipelinesKHR;
	};
}