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
	
	void BasePassRenderer::CreateIndirectCommandsLayout()
	{
		SPICES_PROFILE_ZONE;

		std::vector<VkIndirectCommandsLayoutTokenNV> inputInfos;
		
		uint32_t numInputs = 0;

		{
			VkIndirectCommandsLayoutTokenNV input{};
			input.sType     = VK_STRUCTURE_TYPE_INDIRECT_COMMANDS_LAYOUT_TOKEN_NV;
			input.tokenType = VK_INDIRECT_COMMANDS_TOKEN_TYPE_SHADER_GROUP_NV;
			
			input.stream =  numInputs;
			input.offset =  0;
			inputInfos.push_back(input);
			m_IndirectData.inputStrides.push_back(sizeof(VkBindShaderGroupIndirectCommandNV));
			numInputs++;
			m_IndirectData.strides += sizeof(VkBindShaderGroupIndirectCommandNV);
		}

		{
			VkIndirectCommandsLayoutTokenNV input{}; 
			input.sType     = VK_STRUCTURE_TYPE_INDIRECT_COMMANDS_LAYOUT_TOKEN_NV;
			input.tokenType = VK_INDIRECT_COMMANDS_TOKEN_TYPE_PUSH_CONSTANT_NV;

			input.pushconstantPipelineLayout   = m_Pipelines["BasePassRenderer.Mesh.Default"]->GetPipelineLayout();
			input.pushconstantShaderStageFlags = VK_SHADER_STAGE_ALL;
			input.pushconstantOffset           = 0;
			input.pushconstantSize             = sizeof(VkDeviceAddress);

			input.stream                       = numInputs;
			input.offset                       = 0;
			inputInfos.push_back(input);
			m_IndirectData.inputStrides.push_back(sizeof(VkDeviceAddress));
			numInputs++;
			m_IndirectData.strides += sizeof(VkDeviceAddress);
		}

		{
			VkIndirectCommandsLayoutTokenNV input{};
			input.sType     = VK_STRUCTURE_TYPE_INDIRECT_COMMANDS_LAYOUT_TOKEN_NV; 
			input.tokenType = VK_INDIRECT_COMMANDS_TOKEN_TYPE_DRAW_TASKS_NV;
			
			input.stream = numInputs;
			input.offset = 0;
			inputInfos.push_back(input);
			m_IndirectData.inputStrides.push_back(sizeof(VkDrawMeshTasksIndirectCommandEXT));
			numInputs++;
			m_IndirectData.strides += sizeof(VkDrawMeshTasksIndirectCommandEXT);
		}

		VkIndirectCommandsLayoutCreateInfoNV genInfo{};
		genInfo.sType           = VK_STRUCTURE_TYPE_INDIRECT_COMMANDS_LAYOUT_CREATE_INFO_NV;
		genInfo.flags           = VK_INDIRECT_COMMANDS_LAYOUT_USAGE_EXPLICIT_PREPROCESS_BIT_NV;
		genInfo.tokenCount      = (uint32_t)inputInfos.size();
		genInfo.pTokens         = inputInfos.data();
		genInfo.streamCount     = numInputs;
		genInfo.pStreamStrides  = m_IndirectData.inputStrides.data();

		m_VulkanState.m_VkFunc.vkCreateIndirectCommandsLayoutNV(m_VulkanState.m_Device, &genInfo, NULL, &m_IndirectData.indirectCmdsLayout);
	}

	void BasePassRenderer::OnMeshAddedWorld()
	{
		SPICES_PROFILE_ZONE;
		return;
		std::unordered_map<std::string, uint32_t> pipelineMap;

		int i = 0;
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
				i++;

				return false;
			});
		}

		m_PipelinesRef["Mesh"].resize(pipelineMap.size());

		for (auto& pair : pipelineMap)
		{
			m_PipelinesRef["Mesh"][pair.second] = m_Pipelines[pair.first]->GetPipeline();
		}

		VulkanBuffer stagingBuffer(
			m_VulkanState,
			i * m_IndirectData.strides,
			0,
			0
		);

		for (auto& e : view)
		{
			auto& meshComp = FrameInfo::Get().m_World->GetRegistry().get<MeshComponent>(e);

			meshComp.GetMesh()->GetPacks().for_each([&](const auto& k, const std::shared_ptr<MeshPack>& v) {

				VkBindShaderGroupIndirectCommandNV shader;
				shader.groupIndex = v->GetShaderGroupHandle();
				stagingBuffer.WriteToBuffer(&shader, sizeof(VkBindShaderGroupIndirectCommandNV), i * m_IndirectData.inputStrides[0]);

				//stagingBuffer.WriteToBuffer(&address, sizeof(VkDeviceAddress), i * inputStrides[1] + nMeshPack * strides);

				VkDrawMeshTasksIndirectCommandEXT drawCommand{};
				drawCommand.groupCountX = v->GetNTasks();
				drawCommand.groupCountY = 1;
				drawCommand.groupCountZ = 1;
				stagingBuffer.WriteToBuffer(&drawCommand, sizeof(VkDrawMeshTasksIndirectCommandEXT), i * m_IndirectData.inputStrides[2] + 2 * i * m_IndirectData.strides);

				return false;
			});
		}

		VkIndirectCommandsStreamNV input;
		input.buffer = m_IndirectData.inputBuffer->Get();
		
		{
			input.offset = 0;
			m_IndirectData.inputs.push_back(input);
		}
		{
			input.offset = i * m_IndirectData.strides;
			m_IndirectData.inputs.push_back(input);
		}
		{
			input.offset = 2 * i * m_IndirectData.strides;
			m_IndirectData.inputs.push_back(input);
		}

		Renderer::OnSystemInitialize();
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
		
		builder.BeginRenderPassAsync();

		//builder.BindDescriptorSet(DescriptorSetManager::GetByName({ m_Pass->GetName(), "Mesh" }));

		IterWorldCompSubmitCmdParallel<MeshComponent>(frameInfo, builder.GetSubpassIndex(), [&](VkCommandBuffer& cmdBuffer, int entityId, TransformComponent& transComp, MeshComponent& meshComp) {

			builder.SetViewPort(cmdBuffer);

			builder.BindDescriptorSet(DescriptorSetManager::GetByName("PreRenderer"), cmdBuffer);

			builder.BindDescriptorSet(DescriptorSetManager::GetByName({ m_Pass->GetName(), "Mesh" }), cmdBuffer);

			meshComp.GetMesh()->DrawMeshTasks(cmdBuffer, [&](const uint32_t& meshpackId, const auto& meshPack) {

				builder.BindPipeline(meshPack->GetMaterial()->GetName(), cmdBuffer);

				builder.UpdatePushConstant<uint64_t>([&](auto& push) {
					push = meshPack->GetMeshDesc().GetBufferAddress();
				}, cmdBuffer);
			});
		});
		
		VkGeneratedCommandsInfoNV            info{};
		info.sType                         = VK_STRUCTURE_TYPE_GENERATED_COMMANDS_INFO_NV;
		info.pipeline                      = m_Pipelines["BasePassRenderer.Mesh.Default.DGC"]->GetPipeline();
		info.pipelineBindPoint             = VK_PIPELINE_BIND_POINT_GRAPHICS;
		info.indirectCommandsLayout        = m_IndirectData.indirectCmdsLayout;
		//info.sequencesCount                = m_IndirectData.sequencesCount;
		info.streamCount                   = (uint32_t)m_IndirectData.inputs.size();
		info.pStreams                      = m_IndirectData.inputs.data();
		//info.preprocessBuffer              = m_IndirectData.preprocessBuffer;
		//info.preprocessSize                = m_IndirectData.preprocessSize;

		//m_VulkanState.m_VkFunc.vkCmdExecuteGeneratedCommandsNV(m_VulkanState.m_GraphicCommandBuffer[frameInfo.m_FrameIndex], false, &info);

		builder.BeginNextSubPass("SkyBox");

		builder.SetViewPort();

		builder.BindDescriptorSet(DescriptorSetManager::GetByName("PreRenderer"));

		builder.BindDescriptorSet(DescriptorSetManager::GetByName({ m_Pass->GetName(), "SkyBox" }));

		/*IterWorldCompWithBreak<SkyBoxComponent>(frameInfo, [&](int entityId, TransformComponent& transComp, SkyBoxComponent& skyboxComp) {

			skyboxComp.GetMesh()->DrawMeshTasks(m_VulkanState.m_GraphicCommandBuffer[frameInfo.m_FrameIndex], [&](const uint32_t& meshpackId, const auto& meshPack) {

				builder.BindPipeline(meshPack->GetMaterial()->GetName());

				builder.UpdatePushConstant<uint64_t>([&](auto& push) {
					push = meshPack->GetMeshDesc().GetBufferAddress();
				});
			});

			return true;
		});*/

		builder.EndRenderPass();
	}
}