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
		.AddPushConstant<SpicesShader::PushConstantMesh>()
		.Build();

		DescriptorSetBuilder{ "SkyBox", this }
		.AddPushConstant<SpicesShader::PushConstantMesh>()
		.Build();
	}

	void BasePassRenderer::OnMeshAddedWorld()
	{
		SPICES_PROFILE_ZONE;

		m_BaseMeshDrawCommandsBuffer.clear();

		auto& map = FrameInfo::Get().m_World->GetBaseMeshMap();
		for (auto& nameMap : map)
		{
			std::vector<VkDrawMeshTasksIndirectCommandEXT> commands(nameMap.second.size());

			int i = 0;
			for (auto& uuidMap : nameMap.second)
			{
				commands[i] = uuidMap.second->GetDrawCommand();
				i++;
			}

			uint64_t bufferSize = i * sizeof(VkDrawMeshTasksIndirectCommandEXT);

			VulkanBuffer stagingBuffer(
				m_VulkanState                        , 
				bufferSize                           ,
				VK_BUFFER_USAGE_TRANSFER_SRC_BIT     ,
				VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT  |
				VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
			);

			stagingBuffer.WriteToBuffer(commands.data());

			std::unique_ptr<VulkanBuffer> commandBuffer = std::make_unique<VulkanBuffer>(
				m_VulkanState                             ,
				bufferSize                                ,
				VK_BUFFER_USAGE_INDIRECT_BUFFER_BIT       |
				VK_BUFFER_USAGE_TRANSFER_DST_BIT          |
				VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT ,
				VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
			);

			commandBuffer->CopyBuffer(stagingBuffer.Get(), commandBuffer->Get(), bufferSize);

			m_BaseMeshDrawCommandsBuffer[nameMap.first] = std::move(commandBuffer);
		}
	}

	std::shared_ptr<VulkanPipeline> BasePassRenderer::CreatePipeline(
		std::shared_ptr<Material> material,
		VkPipelineLayout& layout,
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

	void BasePassRenderer::Render(TimeStep& ts, FrameInfo& frameInfo)
	{
		SPICES_PROFILE_ZONE;

		if(frameInfo.m_RendererType != RendererType::Rasterization) return;
		
		RenderBehaveBuilder builder{ this ,frameInfo.m_FrameIndex, frameInfo.m_Imageindex };
		
		builder.BeginRenderPassAsync();

		//builder.BindDescriptorSet(DescriptorSetManager::GetByName({ m_Pass->GetName(), "Mesh" }));

		IterWorldCompSubmitCmdParallel<MeshComponent>(frameInfo, builder.GetSubpassIndex(), [&](VkCommandBuffer& cmdBuffer, int entityId, TransformComponent& transComp, MeshComponent& meshComp) {
			const glm::mat4& modelMatrix = transComp.GetModelMatrix();

			builder.SetViewPort(cmdBuffer);

			builder.BindDescriptorSet(DescriptorSetManager::GetByName("PreRenderer"), cmdBuffer);

			builder.BindDescriptorSet(DescriptorSetManager::GetByName({ m_Pass->GetName(), "Mesh" }), cmdBuffer);

			meshComp.GetMesh()->DrawMeshTasks(cmdBuffer, [&](const uint32_t& meshpackId, const auto& meshPack) {

				builder.BindPipeline(meshPack->GetMaterial()->GetName(), cmdBuffer);

				builder.UpdatePushConstant<SpicesShader::PushConstantMesh>([&](auto& push) {
					push.model                          = modelMatrix;
					push.desc.vertexAddress             = meshPack->GetVerticesBufferAddress();
					push.desc.indexAddress              = meshPack->GetIndicesBufferAddress();
					push.desc.materialParameterAddress  = meshPack->GetMaterial()->GetMaterialParamsAddress();
					push.desc.meshletAddress            = meshPack->GetMeshletsBufferAddress();
					push.desc.verticesCount             = static_cast<uint32_t>(meshPack->GetVertices().size());
					push.desc.indicesCount              = static_cast<uint32_t>(meshPack->GetIndices().size()) / 3;
					push.desc.meshletsCount             = static_cast<uint32_t>(meshPack->GetMeshlets().size());
					push.desc.entityID                  = entityId;
				}, cmdBuffer);
			});
		});

		builder.BeginNextSubPass("SkyBox");

		builder.SetViewPort();

		builder.BindDescriptorSet(DescriptorSetManager::GetByName("PreRenderer"));

		builder.BindDescriptorSet(DescriptorSetManager::GetByName({ m_Pass->GetName(), "SkyBox" }));

		IterWorldCompWithBreak<SkyBoxComponent>(frameInfo, [&](int entityId, TransformComponent& transComp, SkyBoxComponent& skyboxComp) {
			const glm::mat4& modelMatrix = transComp.GetModelMatrix();

			skyboxComp.GetMesh()->DrawMeshTasks(m_VulkanState.m_GraphicCommandBuffer[frameInfo.m_FrameIndex], [&](const uint32_t& meshpackId, const auto& meshPack) {

				builder.BindPipeline(meshPack->GetMaterial()->GetName());

				builder.UpdatePushConstant<SpicesShader::PushConstantMesh>([&](auto& push) {
					push.model                          = modelMatrix;
					push.desc.vertexAddress             = meshPack->GetVerticesBufferAddress();
					push.desc.indexAddress              = meshPack->GetIndicesBufferAddress();
					push.desc.materialParameterAddress  = meshPack->GetMaterial()->GetMaterialParamsAddress();
					push.desc.meshletAddress            = meshPack->GetMeshletsBufferAddress();
					push.desc.verticesCount             = static_cast<unsigned int>(meshPack->GetVertices().size());
					push.desc.indicesCount              = static_cast<unsigned int>(meshPack->GetIndices().size()) / 3;
					push.desc.meshletsCount             = static_cast<uint32_t>(meshPack->GetMeshlets().size());
					push.desc.entityID                  = entityId;
				});
			});

			return true;
		});

		builder.EndRenderPass();
	}
}