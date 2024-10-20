/**
* @file VulkanPipeline.h.
* @brief The VulkanPipeline Class Definitions.
* @author Spices.
*/

#pragma once
#include "Core/Core.h"
#include "VulkanUtils.h"

namespace Spices {

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

		std::vector<VkVertexInputBindingDescription>     bindingDescriptions{};    /* @brief VertexInputBindingDescription, from Vertex.   */
		std::vector<VkVertexInputAttributeDescription>   attributeDescriptions{};  /* @brief VertexInputAttributeDescription, from Vertex. */
		VkViewport                                       viewport{};               /* @brief Viewport.                                     */
		VkRect2D                                         scissor{};                /* @brief Scissor.                                      */
		VkPipelineViewportStateCreateInfo                viewportInfo{};           /* @brief VkPipelineViewportStateCreateInfo.            */
		VkPipelineInputAssemblyStateCreateInfo           inputAssemblyInfo{};      /* @brief VkPipelineInputAssemblyStateCreateInfo.       */
		VkPipelineRasterizationStateCreateInfo           rasterizationInfo{};      /* @brief VkPipelineRasterizationStateCreateInfo.       */
		VkPipelineMultisampleStateCreateInfo             multisampleInfo{};        /* @brief VkPipelineMultisampleStateCreateInfo.         */
		VkPipelineColorBlendStateCreateInfo              colorBlendInfo{};         /* @brief VkPipelineColorBlendStateCreateInfo.          */
		VkPipelineDepthStencilStateCreateInfo            depthStencilInfo{};       /* @brief VkPipelineDepthStencilStateCreateInfo.        */
		std::vector<VkDynamicState>                      dynamicStateEnables;      /* @brief VkDynamicState.                               */
		VkPipelineDynamicStateCreateInfo                 dynamicStateInfo{};       /* @brief VkPipelineDynamicStateCreateInfo.             */
		VkPipelineLayout                                 pipelineLayout{};         /* @brief VkPipelineLayout.                             */
		VkRenderPass                                     renderPass{};             /* @brief VkRenderPass.                                 */
		uint32_t                                         subpass = 0;              /* @brief SubPass num.                                  */
	};

	/**
	* @brief This class is a wrapper of VkPipelineLayout and VkPipeline.
	*/
	class VulkanPipeline : public VulkanObject
	{
	public:

		/**
		* @brief Using ShaderMap to samplify std::unordered_map<std::string, std::vector<std::string>>.
		*/
		using ShaderMap = std::unordered_map<std::string, std::vector<std::string>>;

	public:

		/**
		* @brief Constructor Function.
		* @param[in] vulkanState The global VulkanState.
		*/
		VulkanPipeline(VulkanState& vulkanState) 
			: VulkanObject(vulkanState) 
		{}

		/**
		* @brief Constructor Function.
		* Create VkPipeline.
		* @param[in] vulkanState The global VulkanState.
		* @param[in] pipelineName The Pipeline name.
		* @param[in] shaders The Shader stage name and path.
		* @param[in] config PipelineConfigInfo.
		*/
		VulkanPipeline(
			VulkanState&               vulkanState   ,
			const std::string&         pipelineName  ,
			const ShaderMap&           shaders       ,
			const PipelineConfigInfo&  config
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
		* @brief Get VkPipelineLayout.
		* @return Returns the VkPipelineLayout.
		*/
		VkPipelineLayout& GetPipelineLayout() { return m_PipelineLayout; }

		/**
		* @brief Get VkPipeline.
		* @return Returns the VkPipeline.
		*/
		VkPipeline& GetPipeline() { return m_Pipeline; }

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
	* @brief This class is a wrapper of RayTracing Pipeline.
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
			VulkanState&               vulkanState   ,
			const std::string&         pipelineName  ,
			const ShaderMap&           shaders       ,
			const PipelineConfigInfo&  config
		);

		/**
		* @brief Destructor Function.
		*/
		virtual ~VulkanRayTracingPipeline() override = default;
	};

	/**
	* @brief This class is a wrapper of Compute Pipeline.
	*/
	class VulkanComputePipeline : public VulkanPipeline
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
		VulkanComputePipeline(
			VulkanState&               vulkanState    ,
			const std::string&         pipelineName   ,
			const ShaderMap&           shaders        ,
			const PipelineConfigInfo&  config
		);

		/**
		* @brief Destructor Function.
		*/
		virtual ~VulkanComputePipeline() override = default;
	};

	/**
	* @brief This class is a wrapper of Mesh Pipeline.
	*/
	class VulkanMeshPipeline : public VulkanPipeline
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
		VulkanMeshPipeline(
			VulkanState&               vulkanState    ,
			const std::string&         pipelineName   ,
			const ShaderMap&           shaders        ,
			const PipelineConfigInfo&  config
		);

		/**
		* @brief Destructor Function.
		*/
		virtual ~VulkanMeshPipeline() override = default;
	};

	/**
	* @brief This class is a wrapper of Indirect Pipeline.
	*/
	class VulkanIndirectPipelineNV : public VulkanPipeline
	{
	public:

		/**
		* @brief Constructor Function.
		* Create VkPipeline.
		* @param[in] vulkanState The global VulkanState.
		* @param[in] pipelineName The Pipeline name.
		* @param[in] materialName The Reference Material name.
		* @param[in] pipelineRef The pipeline Reference.
		* @param[in] config PipelineConfigInfo.
		*/
		VulkanIndirectPipelineNV(
			VulkanState&                    vulkanState   ,
			const std::string&              pipelineName  ,
			const std::string&              materialName  ,
			const std::vector<VkPipeline>&  pipelineRef   ,
			const PipelineConfigInfo&       config
		);

		/**
		* @brief Destructor Function.
		*/
		virtual ~VulkanIndirectPipelineNV() override = default;
	};

	/**
	* @brief This class is a wrapper of Indirect Mesh Pipeline.
	*/
	class VulkanIndirectMeshPipelineNV : public VulkanPipeline
	{
	public:

		/**
		* @brief Constructor Function.
		* Create VkPipeline.
		* @param[in] vulkanState The global VulkanState.
		* @param[in] pipelineName The Pipeline name.
		* @param[in] materialName The Reference Material name.
		* @param[in] pipelineRef The pipeline Reference.
		* @param[in] config PipelineConfigInfo.
		*/
		VulkanIndirectMeshPipelineNV(
			VulkanState&                    vulkanState   ,
			const std::string&              pipelineName  ,
			const std::string&              materialName  ,
			const std::vector<VkPipeline>&  pipelineRef   ,
			const PipelineConfigInfo&       config
		);

		/**
		* @brief Destructor Function.
		*/
		virtual ~VulkanIndirectMeshPipelineNV() override = default;
	};
}