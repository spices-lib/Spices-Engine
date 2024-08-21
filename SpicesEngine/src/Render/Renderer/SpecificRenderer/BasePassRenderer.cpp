/**
* @file BasePassRenderer.cpp.
* @brief The BasePassRenderer Class Implementation.
* @author Spices.
*/

#include "Pchheader.h"
#include "BasePassRenderer.h"

namespace Spices {

	void BasePassRenderer::CreateRendererPass()
	{
		SPICES_PROFILE_ZONE;

		RendererPassBuilder{ "BassPass", this }
		.AddSubPass("Mesh")
		.AddColorAttachment("Albedo", TextureType::Texture2D, [](bool& isEnableBlend, VkAttachmentDescription& description) {
			description.initialLayout         = VK_IMAGE_LAYOUT_UNDEFINED;
			description.loadOp                = VK_ATTACHMENT_LOAD_OP_CLEAR;
		})
		.AddColorAttachment("Normal", TextureType::Texture2D, [](bool& isEnableBlend, VkAttachmentDescription& description) {
			description.initialLayout         = VK_IMAGE_LAYOUT_UNDEFINED;
			description.loadOp                = VK_ATTACHMENT_LOAD_OP_CLEAR;
		})
		.AddColorAttachment("Roughness", TextureType::Texture2D, [](bool& isEnableBlend, VkAttachmentDescription& description) {
			description.initialLayout         = VK_IMAGE_LAYOUT_UNDEFINED;
			description.loadOp                = VK_ATTACHMENT_LOAD_OP_CLEAR;
		})
		.AddColorAttachment("Metallic", TextureType::Texture2D, [](bool& isEnableBlend, VkAttachmentDescription& description) {
			description.initialLayout         = VK_IMAGE_LAYOUT_UNDEFINED;
			description.loadOp                = VK_ATTACHMENT_LOAD_OP_CLEAR;
		})
		.AddColorAttachment("Position", TextureType::Texture2D, [](bool& isEnableBlend, VkAttachmentDescription& description) {
			description.initialLayout         = VK_IMAGE_LAYOUT_UNDEFINED;
			description.loadOp                = VK_ATTACHMENT_LOAD_OP_CLEAR;
			description.format                = VK_FORMAT_R32G32B32A32_SFLOAT;
		})
		.AddColorAttachment("ID", TextureType::Texture2D, [](bool& isEnableBlend, VkAttachmentDescription& description) {
			description.initialLayout         = VK_IMAGE_LAYOUT_UNDEFINED;
			description.loadOp                = VK_ATTACHMENT_LOAD_OP_CLEAR;
			description.format                = VK_FORMAT_R32_SFLOAT;
		})
		.AddDepthAttachment("Depth", TextureType::Texture2D, [](VkAttachmentDescription& description) {
			description.initialLayout         = VK_IMAGE_LAYOUT_UNDEFINED;   /* @attention It seams that layout transform is not work? */
			description.loadOp                = VK_ATTACHMENT_LOAD_OP_CLEAR;
		})
		.EndSubPass()
		.AddSubPass("SkyBox")
		.AddColorAttachment("Albedo", TextureType::Texture2D, [](bool& isEnableBlend, VkAttachmentDescription& description) {})
		.AddColorAttachment("Position", TextureType::Texture2D, [](bool& isEnableBlend, VkAttachmentDescription& description) {
			description.format                = VK_FORMAT_R32G32B32A32_SFLOAT;
		})
		.AddColorAttachment("ID", TextureType::Texture2D, [](bool& isEnableBlend, VkAttachmentDescription& description) {
			description.format                = VK_FORMAT_R32_SFLOAT;
		})
		.AddDepthAttachment("Depth", TextureType::Texture2D, [](VkAttachmentDescription& description) {})
		.EndSubPass()
		.Build();
	}

	void BasePassRenderer::CreateDescriptorSet()
	{
		SPICES_PROFILE_ZONE;

		DescriptorSetBuilder{ "Mesh", this }
		.AddPushConstant(sizeof(uint64_t))
		.Build();

		DescriptorSetBuilder{ "SkyBox", this }
		.AddPushConstant(sizeof(uint64_t))
		.Build();
	}
	
	void BasePassRenderer::CreateDeviceGeneratedCommandsLayout()
	{
		SPICES_PROFILE_ZONE;

		DGCLayoutBuilder{ "Mesh", this }
		.AddShaderGroupInput()
		.AddPushConstantInput()
		.AddDrawMeshTaskInput()
		.Build();
	}

	void BasePassRenderer::OnMeshAddedWorld()
	{
		SPICES_PROFILE_ZONE;

		m_IndirectData.ResetInput();

		std::unordered_map<std::string, uint32_t> pipelineMap;

		auto view = FrameInfo::Get().m_World->GetRegistry().view<MeshComponent>();
		for (auto& e : view)
		{
			auto& meshComp = FrameInfo::Get().m_World->GetRegistry().get<MeshComponent>(e);

			meshComp.GetMesh()->GetPacks().for_each([&](const auto& k, const std::shared_ptr<MeshPack>& v) {
				if (pipelineMap.find(v->GetMaterial()->GetName()) == pipelineMap.end())
				{
					pipelineMap[v->GetMaterial()->GetName()] = pipelineMap.size();
				}
				v->SetShaderGroupHandle(pipelineMap[v->GetMaterial()->GetName()]);
				m_IndirectData.nMeshPack++;
				return false;
			});
		}

		m_PipelinesRef["Mesh"].resize(pipelineMap.size());

		for (auto& pair : pipelineMap)
		{
			m_PipelinesRef["Mesh"][pair.second] = m_Pipelines[pair.first]->GetPipeline();
		}

		VkPhysicalDeviceDeviceGeneratedCommandsPropertiesNV genProps{};
		genProps.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DEVICE_GENERATED_COMMANDS_PROPERTIES_NV;

		VkPhysicalDeviceProperties2 phyProps{};
		phyProps.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PROPERTIES_2;
		phyProps.pNext = &genProps;
		
		vkGetPhysicalDeviceProperties2(m_VulkanState.m_PhysicalDevice, &phyProps);

		size_t alignSeqIndexMask = genProps.minSequencesIndexBufferOffsetAlignment - 1;
		size_t alignMask = genProps.minIndirectCommandsBufferOffsetAlignment - 1;

		size_t totalSize = 0;
		size_t pipeOffset = totalSize;
		totalSize = totalSize + ((sizeof(VkBindShaderGroupIndirectCommandNV) * m_IndirectData.nMeshPack + alignMask) & (~alignMask));
		size_t pushOffset = totalSize;
		totalSize = totalSize + ((sizeof(VkDeviceAddress) * m_IndirectData.nMeshPack + alignMask) & (~alignMask));
		size_t drawOffset = totalSize;
		totalSize = totalSize + ((sizeof(VkDrawMeshTasksIndirectCommandNV) * m_IndirectData.nMeshPack + alignMask) & (~alignMask));


		VulkanBuffer stagingBuffer(
			m_VulkanState,
			totalSize,
			VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
			VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
		);

		int i = 0;
		for (auto& e : view)
		{
			auto& meshComp = FrameInfo::Get().m_World->GetRegistry().get<MeshComponent>(e);

			meshComp.GetMesh()->GetPacks().for_each([&](const auto& k, const std::shared_ptr<MeshPack>& v) {
				VkBindShaderGroupIndirectCommandNV shader{ v->GetShaderGroupHandle() + 1 };
				std::cout << shader.groupIndex << std::endl;
				stagingBuffer.WriteToBuffer(&shader, sizeof(VkBindShaderGroupIndirectCommandNV), i * m_IndirectData.inputStrides[0] + pipeOffset);
				
				VkDeviceAddress push{ v->GetMeshDesc().GetBufferAddress() };
				stagingBuffer.WriteToBuffer(&push, sizeof(VkDeviceAddress),                      i * m_IndirectData.inputStrides[1] + pushOffset);

				VkDrawMeshTasksIndirectCommandNV drawCmd{ v->GetDrawCommand() };
				stagingBuffer.WriteToBuffer(&drawCmd, sizeof(VkDrawMeshTasksIndirectCommandNV), i * m_IndirectData.inputStrides[2] + drawOffset);

				i++;
				return false;
			});
		}
		stagingBuffer.Flush();

		m_IndirectData.inputBuffer = std::make_unique<VulkanBuffer>(
			m_VulkanState,
			totalSize,
			VK_BUFFER_USAGE_TRANSFER_DST_BIT    ,
			VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
		);

		m_IndirectData.inputBuffer->CopyBuffer(stagingBuffer.Get(), m_IndirectData.inputBuffer->Get(), totalSize);

		VkIndirectCommandsStreamNV input;
		input.buffer = m_IndirectData.inputBuffer->Get();
		
		{
			input.offset = pipeOffset;
			m_IndirectData.inputs.push_back(input);
		}
		{
			input.offset = pushOffset;
			m_IndirectData.inputs.push_back(input);
		}
		{
			input.offset = drawOffset;
			m_IndirectData.inputs.push_back(input);
		}

		Renderer::OnSystemInitialize();

		VkGeneratedCommandsMemoryRequirementsInfoNV memInfo{};
		memInfo.sType = VK_STRUCTURE_TYPE_GENERATED_COMMANDS_MEMORY_REQUIREMENTS_INFO_NV;
		memInfo.maxSequencesCount = m_IndirectData.nMeshPack;
		memInfo.indirectCommandsLayout = m_IndirectData.indirectCmdsLayout;
		memInfo.pipeline = m_Pipelines["BasePassRenderer.Mesh.Default.DGC"]->GetPipeline();
		memInfo.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;

		VkMemoryRequirements2 memReqs{};
		memReqs.sType = VK_STRUCTURE_TYPE_MEMORY_REQUIREMENTS_2;
		m_VulkanState.m_VkFunc.vkGetGeneratedCommandsMemoryRequirementsNV(m_VulkanState.m_Device, &memInfo, &memReqs);

		m_IndirectData.preprocessSize = memReqs.memoryRequirements.size;
		m_IndirectData.preprocessBuffer = std::make_unique<VulkanBuffer>(
			m_VulkanState,
			m_IndirectData.preprocessSize,
			VK_BUFFER_USAGE_INDIRECT_BUFFER_BIT,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT  |
			VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
		);

		
	}

	std::shared_ptr<VulkanPipeline> BasePassRenderer::CreatePipeline(
		std::shared_ptr<Material>        material ,
		VkPipelineLayout&                layout   ,
		std::shared_ptr<RendererSubPass> subPass
	)
	{
		SPICES_PROFILE_ZONE;

		PipelineConfigInfo pipelineConfig{};
		VulkanPipeline::DefaultPipelineConfigInfo(pipelineConfig);

		pipelineConfig.renderPass                      = m_Pass->Get();
		pipelineConfig.subpass                         = subPass->GetIndex();
		pipelineConfig.pipelineLayout                  = layout;
		pipelineConfig.rasterizationInfo.cullMode      = VK_CULL_MODE_NONE;
		pipelineConfig.colorBlendInfo.attachmentCount  = static_cast<uint32_t>(subPass->GetColorBlend().size());
		pipelineConfig.colorBlendInfo.pAttachments     = subPass->GetColorBlend().data();

		return std::make_shared<VulkanMeshPipeline>(
			m_VulkanState,
			material->GetName(),
			material->GetShaderPath(),
			pipelineConfig
		);
	}

	std::shared_ptr<VulkanPipeline> BasePassRenderer::CreateDGCPipeline(
		const std::string&               pipelineName ,
		const std::string&               materialName ,
		VkPipelineLayout&                layout       ,
		std::shared_ptr<RendererSubPass> subPass
	)
	{
		SPICES_PROFILE_ZONE;

		/**
		* @brief Get Dafault PipelineConfigInfo.
		*/
		PipelineConfigInfo pipelineConfig{};
		VulkanPipeline::DefaultPipelineConfigInfo(pipelineConfig);

		/**
		* @brief Fill in with configurable data.
		*/
		pipelineConfig.renderPass                     = m_Pass->Get();
		pipelineConfig.subpass                        = subPass->GetIndex();
		pipelineConfig.pipelineLayout                 = layout;
		pipelineConfig.colorBlendInfo.attachmentCount = static_cast<uint32_t>(subPass->GetColorBlend().size());
		pipelineConfig.colorBlendInfo.pAttachments    = subPass->GetColorBlend().data();

		/**
		* @brief Create VulkanPipeline.
		*/
		return std::make_shared<VulkanIndirectMeshPipelineNV>(
			m_VulkanState,
			pipelineName,
			materialName,
			m_PipelinesRef[subPass->GetName()],
			pipelineConfig
		);

		m_PipelinesRef.clear();
	}

	void BasePassRenderer::Render(TimeStep& ts, FrameInfo& frameInfo)
	{
		SPICES_PROFILE_ZONE;

		if(frameInfo.m_RendererType != RendererType::Rasterization) return;
		
		RenderBehaveBuilder builder{ this ,frameInfo.m_FrameIndex, frameInfo.m_Imageindex };
		
		builder.BeginRenderPass();

		builder.SetViewPort();

		builder.BindDescriptorSet(DescriptorSetManager::GetByName("PreRenderer"));

		builder.BindDescriptorSet(DescriptorSetManager::GetByName({ m_Pass->GetName(), "Mesh" }));

		builder.PreprocessDGC_NV();

		builder.PipelineMemoryBarrier(
			VK_ACCESS_COMMAND_PREPROCESS_WRITE_BIT_NV   , 
			VK_ACCESS_INDIRECT_COMMAND_READ_BIT         , 
			VK_PIPELINE_STAGE_COMMAND_PREPROCESS_BIT_NV , 
			VK_PIPELINE_STAGE_DRAW_INDIRECT_BIT
		);

		builder.ExecuteDGC_NV();

		builder.BeginNextSubPass("SkyBox");

		builder.SetViewPort();

		builder.BindDescriptorSet(DescriptorSetManager::GetByName("PreRenderer"));

		builder.BindDescriptorSet(DescriptorSetManager::GetByName({ m_Pass->GetName(), "SkyBox" }));

		IterWorldCompWithBreak<SkyBoxComponent>(frameInfo, [&](int entityId, TransformComponent& transComp, SkyBoxComponent& skyboxComp) {

			skyboxComp.GetMesh()->DrawMeshTasks(m_VulkanState.m_GraphicCommandBuffer[frameInfo.m_FrameIndex], [&](const uint32_t& meshpackId, const auto& meshPack) {

				builder.BindPipeline(meshPack->GetMaterial()->GetName());

				builder.UpdatePushConstant<uint64_t>([&](auto& push) {
					push = meshPack->GetMeshDesc().GetBufferAddress();
				});
			});

			return true;
		});

		builder.EndRenderPass();
	}
}