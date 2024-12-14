/**
* @file VulkanDeviceGeneratedCommandsNV.cpp.
* @brief The VulkanDeviceGeneratedCommandsNV Class Implementation.
* @author Spices.
*/

#include "Pchheader.h"
#include "VulkanDeviceGeneratedCommandsNV.h"

namespace Spices {

	VulkanDeviceGeneratedCommandsNV::VulkanDeviceGeneratedCommandsNV(VulkanState& vulkanState)
		: VulkanObject(vulkanState)
		, m_IndirectCmdsLayout(VK_NULL_HANDLE)
		, m_InputBuffer(nullptr)
		, m_PreprocessBuffer(nullptr)
		, m_NSequence(0)
		, m_PreprocessSize(0)
		, m_Strides(0)
		, m_InputStrides{}
		, m_LayoutTokens{}
		, m_InputStreams{}
	{}

	VulkanDeviceGeneratedCommandsNV::~VulkanDeviceGeneratedCommandsNV()
	{}

	void VulkanDeviceGeneratedCommandsNV::ResetCommandsLayout()
	{
		SPICES_PROFILE_ZONE;

		m_InputStrides.clear();
		m_Strides           = 0;
		m_IndirectCmdsLayout = nullptr;

	}

	void VulkanDeviceGeneratedCommandsNV::ResetInput()
	{
		SPICES_PROFILE_ZONE;

		m_NSequence         = 0;
		m_InputBuffer       = nullptr;
		m_InputStreams.clear();
		m_PreprocessBuffer  = nullptr;
		m_PreprocessSize    = 0;
	}

	void VulkanDeviceGeneratedCommandsNV::AddInputStride(uint32_t stride)
	{
		SPICES_PROFILE_ZONE;

		m_InputStrides.push_back(stride);
		m_Strides += stride;
	}

	std::shared_ptr<VulkanBuffer> VulkanDeviceGeneratedCommandsNV::CreateInputBuffer(uint32_t size)
	{
		SPICES_PROFILE_ZONE;

		m_InputBuffer = std::make_shared<VulkanBuffer>(
			m_VulkanState,
			"GDCInputBuffer",
			size,
			VK_BUFFER_USAGE_TRANSFER_DST_BIT    ,
			VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
		);

		return m_InputBuffer;
	}

	std::shared_ptr<VulkanBuffer> VulkanDeviceGeneratedCommandsNV::CreatePreprocessBuffer(uint32_t size)
	{
		SPICES_PROFILE_ZONE;

		m_PreprocessBuffer = std::make_shared<VulkanBuffer>(
			m_VulkanState,
			"GDCPreprocessBuffer",
			size,
			VK_BUFFER_USAGE_INDIRECT_BUFFER_BIT,
			VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
		);

		return m_PreprocessBuffer;
	}

	void VulkanDeviceGeneratedCommandsNV::BuildCommandLayout(const std::vector<VkIndirectCommandsLayoutTokenNV>& inputInfos)
	{
		SPICES_PROFILE_ZONE;

		m_LayoutTokens = inputInfos;

		/**
		* @brief Instance a VkIndirectCommandsLayoutCreateInfoNV.
		*/
		VkIndirectCommandsLayoutCreateInfoNV     genInfo{};
		genInfo.sType                          = VK_STRUCTURE_TYPE_INDIRECT_COMMANDS_LAYOUT_CREATE_INFO_NV;
		genInfo.flags                          = VK_INDIRECT_COMMANDS_LAYOUT_USAGE_UNORDERED_SEQUENCES_BIT_NV;
		genInfo.tokenCount                     = static_cast<uint32_t>(inputInfos.size());
		genInfo.pTokens                        = inputInfos.data();
		genInfo.streamCount                    = static_cast<uint32_t>(m_InputStrides.size());
		genInfo.pStreamStrides                 = m_InputStrides.data();

		m_IndirectCmdsLayout = std::make_shared<VulkanIndirectCommmandsLayoutNV>(m_VulkanState, genInfo);
	}

	void VulkanDeviceGeneratedCommandsNV::PreprocessDGC(const VkCommandBuffer& cmdBuffer, VkPipeline pipeline) const
	{
		SPICES_PROFILE_ZONE;

		if (m_NSequence == 0) return;

		/**
		* @brief Instance a VkGeneratedCommandsInfoNV.
		*/
		VkGeneratedCommandsInfoNV            info{};
		info.sType                         = VK_STRUCTURE_TYPE_GENERATED_COMMANDS_INFO_NV;
		info.pipeline                      = pipeline;
		info.pipelineBindPoint             = VK_PIPELINE_BIND_POINT_GRAPHICS;
		info.indirectCommandsLayout        = m_IndirectCmdsLayout->Get();
		info.sequencesCount                = m_NSequence;
		info.streamCount                   = static_cast<uint32_t>(m_InputStreams.size());
		info.pStreams                      = m_InputStreams.data();
		info.preprocessBuffer              = m_PreprocessBuffer->Get();
		info.preprocessSize                = m_PreprocessSize;

		/**
		* @brief Call vkCmdPreprocessGeneratedCommandsNV.
		*/
		m_VulkanState.m_VkFunc.vkCmdPreprocessGeneratedCommandsNV(cmdBuffer, &info);
	}

	void VulkanDeviceGeneratedCommandsNV::ExecuteDGC(const VkCommandBuffer& cmdBuffer, VkPipeline pipeline) const
	{
		SPICES_PROFILE_ZONE;

		if (m_NSequence == 0) return;

		/**
		* @brief Instance a VkGeneratedCommandsInfoNV.
		*/
		VkGeneratedCommandsInfoNV            info{};
		info.sType                         = VK_STRUCTURE_TYPE_GENERATED_COMMANDS_INFO_NV;
		info.pipeline                      = pipeline;
		info.pipelineBindPoint             = VK_PIPELINE_BIND_POINT_GRAPHICS;
		info.indirectCommandsLayout        = m_IndirectCmdsLayout->Get();
		info.sequencesCount                = m_NSequence;
		info.streamCount                   = static_cast<uint32_t>(m_InputStreams.size());
		info.pStreams                      = m_InputStreams.data();
		info.preprocessBuffer              = m_PreprocessBuffer->Get();
		info.preprocessSize                = m_PreprocessSize;

		/**
		* @brief Call vkCmdExecuteGeneratedCommandsNV.
		*/
		m_VulkanState.m_VkFunc.vkCmdExecuteGeneratedCommandsNV(cmdBuffer, true, &info);
	}
}