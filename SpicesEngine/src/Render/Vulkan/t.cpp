//#include "Core/Core.h"
//#include "Render/Vulkan/VulkanRenderBackend.h"
//
//using namespace Spices;
//const int nMeshPack = 10;
//
//struct DrawSequence
//{
//	VkBindShaderGroupIndirectCommandNV shader;
//	VkDeviceAddress                    meshdesc;
//	VkDrawMeshTasksIndirectCommandEXT  draw;
//};
//
//struct DrawSetup
//{
//	std::vector<VkIndirectCommandsStreamNV> inputs;
//
//	VkIndirectCommandsLayoutNV indirectCmdsLayout;
//
//	VkBuffer inputBuffer;
//	size_t   inputSequenceIndexOffset;
//
//	VkBuffer     preprocessBuffer;
//	VkDeviceSize preprocessSize;
//
//	uint32_t sequencesCount;
//
//	VkCommandBuffer cmdBuffer;
//
//	size_t fboChangeID;
//	size_t pipeChangeID;
//} m_draw;
//
//void Test() {
//
//VkCommandBuffer cmdBuffer;
//
//std::vector<VkIndirectCommandsLayoutTokenNV> inputInfos;
//std::vector<uint32_t>                        inputStrides;
//uint32_t numInputs = 0;
//uint32_t strides = 0;
//
//{
//	VkIndirectCommandsLayoutTokenNV input{};
//	input.sType     = VK_STRUCTURE_TYPE_INDIRECT_COMMANDS_LAYOUT_TOKEN_NV;
//	input.tokenType = VK_INDIRECT_COMMANDS_TOKEN_TYPE_SHADER_GROUP_NV;
//
//	input.stream =  numInputs;
//	input.offset =  0;
//	inputInfos.push_back(input);
//	inputStrides.push_back(sizeof(VkBindShaderGroupIndirectCommandNV));
//	numInputs++;
//	strides += sizeof(VkBindShaderGroupIndirectCommandNV);
//}
//
//{
//	VkIndirectCommandsLayoutTokenNV input{};
//	input.sType     = VK_STRUCTURE_TYPE_INDIRECT_COMMANDS_LAYOUT_TOKEN_NV; 
//	input.tokenType = VK_INDIRECT_COMMANDS_TOKEN_TYPE_PUSH_CONSTANT_NV;
//
//	input.pushconstantPipelineLayout = res->m_drawPush.getPipeLayout();
//	input.pushconstantShaderStageFlags = VK_SHADER_STAGE_ALL;
//	input.pushconstantOffset = 0;
//	input.pushconstantSize = sizeof(VkDeviceAddress);
//
//	input.stream = numInputs;
//	input.offset = 0;
//	inputInfos.push_back(input);
//	inputStrides.push_back(sizeof(VkDeviceAddress));
//	numInputs++;
//	strides += sizeof(VkDeviceAddress);
//}
//
//{
//	VkIndirectCommandsLayoutTokenNV input{};
//	input.sType     = VK_STRUCTURE_TYPE_INDIRECT_COMMANDS_LAYOUT_TOKEN_NV; 
//	input.tokenType = VK_INDIRECT_COMMANDS_TOKEN_TYPE_DRAW_TASKS_NV;
//
//	input.stream = numInputs;
//	input.offset = 0;
//	inputInfos.push_back(input);
//	inputStrides.push_back(sizeof(VkDrawMeshTasksIndirectCommandEXT));
//	numInputs++;
//	strides += sizeof(VkDrawMeshTasksIndirectCommandEXT);
//}
//
//VkIndirectCommandsLayoutCreateInfoNV genInfo{};
//genInfo.sType           = VK_STRUCTURE_TYPE_INDIRECT_COMMANDS_LAYOUT_CREATE_INFO_NV;
//genInfo.flags           = VK_INDIRECT_COMMANDS_LAYOUT_USAGE_EXPLICIT_PREPROCESS_BIT_NV;
//genInfo.tokenCount      = (uint32_t)inputInfos.size();
//genInfo.pTokens         = inputInfos.data();
//genInfo.streamCount     = numInputs;
//genInfo.pStreamStrides  = inputStrides.data();
//
//vkCreateIndirectCommandsLayoutNV(VulkanRenderBackend::GetState().m_Device, &genInfo, NULL, &m_draw.indirectCmdsLayout);
//
//VulkanBuffer stagingBuffer(
//	VulkanRenderBackend::GetState(),
//	nMeshPack * strides,
//	0,
//	0
//);
//
//for (int i = 0; i < nMeshPack; i++)
//{
//	stagingBuffer.WriteToBuffer(&shaderindex, sizeof(VkBindShaderGroupIndirectCommandNV), i * inputStrides[0]);
//
//	stagingBuffer.WriteToBuffer(&address, sizeof(VkDeviceAddress), i * inputStrides[1] + nMeshPack * strides);
//
//	VkDrawMeshTasksIndirectCommandEXT drawCommand {};
//	drawCommand.groupCountX = nMeshPack->nTasks;
//	drawCommand.groupCountY = 1;
//	drawCommand.groupCountZ = 1;
//	
//	stagingBuffer.WriteToBuffer(&drawCommand, sizeof(VkDrawMeshTasksIndirectCommandEXT), i * inputStrides[2] + 2 * nMeshPack * strides);
//}
//
//VkIndirectCommandsStreamNV input;
//input.buffer = m_draw.inputBuffer;
//
//{
//	input.offset = 0;
//	m_draw.inputs.push_back(input);
//}
//{
//	input.offset = nMeshPack * strides;
//	m_draw.inputs.push_back(input);
//}
//{
//	input.offset = 2 * nMeshPack * strides;
//	m_draw.inputs.push_back(input);
//}
//
//while (1)
//{
//	VkGeneratedCommandsInfoNV            info{};
//	info.sType                         = VK_STRUCTURE_TYPE_GENERATED_COMMANDS_INFO_NV;
//	info.pipeline                      = m_resources->m_drawGroupsPipeline;
//	info.pipelineBindPoint             = VK_PIPELINE_BIND_POINT_GRAPHICS;
//	info.indirectCommandsLayout        = m_draw.indirectCmdsLayout;
//	info.sequencesCount                = m_draw.sequencesCount;
//	info.streamCount                   = (uint32_t)m_draw.inputs.size();
//	info.pStreams                      = m_draw.inputs.data();
//	info.preprocessBuffer              = m_draw.preprocessBuffer;
//	info.preprocessSize                = m_draw.preprocessSize;
//
//	vkCmdPreprocessGeneratedCommandsNV(cmdBuffer, &info);
//	vkCmdExecuteGeneratedCommandsNV(cmdBuffer, true, &info);
//}
//
//}